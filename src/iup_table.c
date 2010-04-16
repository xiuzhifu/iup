/** \file
 * \brief iupTable functions.
 * Implementation by Danny Reinhold and Antonio Scuri. 
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "iup_table.h"
#include "iup_str.h"
#include "iup_assert.h"

/* #define DEBUGTABLE 1 */

/* Adjust these parameters for optimal performance and memory usage */
static const unsigned int itable_maxTableSizeIndex = 8;
static const unsigned int itable_hashTableSize[] = { 31, 101, 401, 1601, 4001, 8009, 16001, 32003, 64007 };
static const float itable_resizeLimit = 2;
static const unsigned int itable_itemGrow = 5;

/* Iteration context.
 */
typedef struct _ItableContext
{
  unsigned int entryIndex;  /* index at the Itable::entries array */
  unsigned int itemIndex;   /* index at the ItableEntry::items array */
} ItableContext;

/* A key of an item.
 * To avoid lots of string comparisions we store
 * a keyindex as an integer.
 * To find a key in an item list we only have to
 * do integer comparisions.
 * Additionally the key itself is stored in
 * keyStr. In a string indexed hashtable this is
 * a duplicated string, in a pointer indexed hash table
 * this is simply the pointer (in this case keyIndex
 * and keyStr are equal).
*/
typedef struct _ItableKey
{
  unsigned long keyIndex;  /* the secondary hash number */
  const char   *keyStr;
}
ItableKey;

/* An item in the hash table.
 * Such an item is stored in the item list of
 * an entry.
 */
typedef struct _ItableItem
{
  Itable_Types  itemType;
  ItableKey          key;
  void              *value;
}
ItableItem;

/* An entry in the hash table.
 * An entry is chosen by an index in the hash table
 * and contains a list of items.
 * The number of items in this list is stored
 * in nextItemIndex.
 * size is the current size of the items array.
 */
typedef struct _ItableEntry
{
  unsigned int  nextItemIndex;
  unsigned int  size;
  ItableItem   *items;
}
ItableEntry;


/* A hash table.
 * indexType is the type of the index.
 * entries is an array of entries. Select an
 * entry by its index.
 * size is the number of entries in the hash table...
 */
struct _Itable
{
  unsigned int            size;
  unsigned int            numberOfEntries;
  unsigned int            tableSizeIndex;  /* index into itable_hashTableSize array */
  Itable_IndexTypes  indexType;
  ItableEntry            *entries;
  ItableContext           context;
};


/* Prototypes of private functions */

static void iTableFreeItemArray(Itable_IndexTypes indexType, unsigned int nextFreeIndex, 
                                                            ItableItem *items);
static unsigned int iTableGetEntryIndex(Itable *it, const char *key, unsigned long *keyIndex);
static unsigned int iTableFindItem(Itable *it, const char *key, ItableEntry **entry, 
                                                          unsigned int *itemIndex,
                                                          unsigned long *keyIndex);
static unsigned int iTableResize(Itable *it);
static void iTableAdd(Itable *it, ItableKey *key, void *value, Itable_Types itemType);
static void iTableUpdateArraySize(ItableEntry *entry);   

#ifdef DEBUGTABLE
static void iTableShowStatistics(Itable *it);
static void iTableCheckDuplicated(ItableItem *item, unsigned int nextItemIndex, 
                                              const char *key, 
                                              unsigned long keyIndex);
#endif


Itable *iupTableCreate(Itable_IndexTypes indexType)
{
  return iupTableCreateSized(indexType, 1);  /* 101 shows to be a better start for IUP */
}


Itable *iupTableCreateSized(Itable_IndexTypes indexType, unsigned int initialSizeIndex)
{
  Itable *it = (Itable *)malloc(sizeof(struct _Itable));

  iupASSERT(it!=NULL);
  if (!it)
    return 0;

  if (initialSizeIndex > itable_maxTableSizeIndex)
    initialSizeIndex = itable_maxTableSizeIndex;

  it->size            = itable_hashTableSize[initialSizeIndex];
  it->tableSizeIndex  = initialSizeIndex;
  it->numberOfEntries = 0;
  it->indexType       = indexType;

  it->entries = (ItableEntry *)malloc(it->size * sizeof(ItableEntry));
  iupASSERT(it->entries!=NULL);
  if (!it->entries)
  {
    free(it);
    return 0;
  }

  memset(it->entries, 0, it->size * sizeof(ItableEntry));

  it->context.entryIndex = (unsigned int)-1;
  it->context.itemIndex = (unsigned int)-1;

  return it;
}

void iupTableClear(Itable *it)
{
  unsigned int i;

  if (!it)
    return;

  for (i = 0; i < it->size; i++)
  {
    ItableEntry *entry = &(it->entries[i]);
    if (entry->items)
      iTableFreeItemArray(it->indexType, entry->nextItemIndex, entry->items);
  }

  it->numberOfEntries = 0;

  memset(it->entries, 0, it->size * sizeof(ItableEntry));

  it->context.entryIndex = (unsigned int)-1;
  it->context.itemIndex = (unsigned int)-1;
}

void iupTableDestroy(Itable *it)
{
  if (!it)
    return;

#ifdef DEBUGTABLE
   iTableShowStatistics(it);
#endif

  iupTableClear(it);

  if (it->entries)
    free(it->entries);

  free(it);
}

int iupTableCount(Itable *it)
{
  iupASSERT(it!=NULL);
  if (!it)
    return 0;
  return it->numberOfEntries;
}

void iupTableSetFunc(Itable *it, const char *key, Ifunc func)
{
  iupTableSet(it, key, (void*)func, IUPTABLE_FUNCPOINTER); /* type cast from function pointer to void* */
}

void iupTableSet(Itable *it, const char *key, void *value, Itable_Types itemType)
{
  unsigned int  itemIndex,
                itemFound;
  unsigned long keyIndex;
  ItableEntry  *entry;
  ItableItem   *item;
  void         *v;

  iupASSERT(it!=NULL);
  iupASSERT(key!=NULL);
  if (!it || !key || !value)
    return;

  itemFound = iTableFindItem(it, key, &entry, &itemIndex, &keyIndex);

#ifdef DEBUGTABLE
  if (it->indexType == IUPTABLE_STRINGINDEXED)
    iTableCheckDuplicated(&(entry->items[0]), entry->nextItemIndex, key, keyIndex);
#endif

  if (!itemFound)
  {
    /* create a new item */

    /* first check if the hash table has to be reorganized */
    if (iTableResize(it))
    {
      /* We have to search for the entry again, since it may
       * have been moved by iTableResize. */
      iTableFindItem(it, key, &entry, &itemIndex, &keyIndex);
    }

    iTableUpdateArraySize(entry);

    /* add the item at the end of the item array */
    if (itemType == IUPTABLE_STRING)
      v = iupStrDup(value);
    else
      v = value;

    item = &(entry->items[entry->nextItemIndex]);

    item->itemType     = itemType;
    item->key.keyIndex = keyIndex;
    item->key.keyStr   = it->indexType == IUPTABLE_STRINGINDEXED? iupStrDup(key) : key;
    item->value        = v;

    entry->nextItemIndex++;
    it->numberOfEntries++;
  }
  else
  {
    /* change an existing item */
    void *v;
    item = &(entry->items[itemIndex]);

    if (itemType == IUPTABLE_STRING && item->itemType == IUPTABLE_STRING)
    {
      /* this will avoid to free + alloc of a new pointer */
      if (iupStrEqual((char*)item->value, (char*)value))
        return;
    }

    if (itemType == IUPTABLE_STRING)
      v = iupStrDup(value);
    else
      v = value;

    if (item->itemType == IUPTABLE_STRING)
      free(item->value);

    item->value    = v;
    item->itemType = itemType;
  }
}

static void iTableRemoveItem(Itable *it, ItableEntry *entry, unsigned int itemIndex)
{
  ItableItem   *item;
  unsigned int  i;

  item = &(entry->items[itemIndex]);

  if (it->indexType == IUPTABLE_STRINGINDEXED)
    free((void *)item->key.keyStr);

  if (item->itemType == IUPTABLE_STRING)
    free(item->value);

  /* re-order the remaining items */
  for (i = itemIndex; i < entry->nextItemIndex-1; i++)
    entry->items[i] = entry->items[i+1];

  /* clear the released item */
  memset(entry->items + entry->nextItemIndex, 0, sizeof (ItableItem));

  entry->nextItemIndex--;
  it->numberOfEntries--;
}

void iupTableRemove(Itable *it, const char *key)
{
  unsigned int itemFound,
               itemIndex;
  unsigned long keyIndex;
  ItableEntry  *entry;

  iupASSERT(it!=NULL);
  iupASSERT(key!=NULL);
  if (!it || !key)
    return;

  itemFound = iTableFindItem(it, key, &entry, &itemIndex, &keyIndex);
  if (itemFound)
    iTableRemoveItem(it, entry, itemIndex);
}

void *iupTableGet(Itable *it, const char *key)
{
  unsigned int  itemFound,
                itemIndex;
  unsigned long keyIndex;
  ItableEntry  *entry;
  void         *value = 0;

  iupASSERT(it!=NULL);
  iupASSERT(key!=NULL);
  if (!it || !key)
    return 0;

  itemFound = iTableFindItem(it, key, &entry, &itemIndex, &keyIndex);
  if (itemFound)
    value = entry->items[itemIndex].value;

  return value;
}

Ifunc iupTableGetFunc(Itable *it, const char *key, void **value)
{
  Itable_Types itemType = IUPTABLE_POINTER;
  *value = iupTableGetTyped(it, key, &itemType);
  if (itemType == IUPTABLE_FUNCPOINTER)
    return (Ifunc)(*value); /* type cast from void* to function pointer */
  else
    return (Ifunc)0;
}

void *iupTableGetTyped(Itable *it, const char *key, Itable_Types *itemType)
{
  unsigned int  itemFound,
                itemIndex;
  unsigned long keyIndex;
  ItableEntry  *entry;
  void         *value = 0;

  iupASSERT(it!=NULL);
  iupASSERT(key!=NULL);
  if (!it || !key)
    return 0;

  itemFound = iTableFindItem(it, key, &entry, &itemIndex, &keyIndex);
  if (itemFound)
  {
    value = entry->items[itemIndex].value;
    if (itemType) 
      *itemType = entry->items[itemIndex].itemType;
  }

  return value;
}

void *iupTableGetCurr(Itable *it)
{
  iupASSERT(it!=NULL);
  if (!it || it->context.entryIndex == (unsigned int)-1
         || it->context.itemIndex == (unsigned int)-1)
    return 0;

  return it->entries[it->context.entryIndex].items[it->context.itemIndex].value;
}


char *iupTableFirst(Itable *it)
{
  unsigned int entryIndex;

  iupASSERT(it!=NULL);
  if (!it)
    return 0;

  it->context.entryIndex = (unsigned int)-1;
  it->context.itemIndex = (unsigned int)-1;

  /* find the first used entry */
  for (entryIndex = 0; entryIndex < it->size; entryIndex++)
  {
    if (it->entries[entryIndex].nextItemIndex > 0)
    {
      it->context.entryIndex = entryIndex;
      it->context.itemIndex = 0;
      return (char*)it->entries[entryIndex].items[0].key.keyStr;
    }
  }

  return 0;
}


char *iupTableNext(Itable *it)
{
  unsigned int entryIndex;

  iupASSERT(it!=NULL);
  if (!it || it->context.entryIndex == (unsigned int)-1
         || it->context.itemIndex == (unsigned int)-1)
    return 0;

  if (it->context.itemIndex + 1 < it->entries[it->context.entryIndex].nextItemIndex)
  {
    /* key in the current entry */
    it->context.itemIndex++;
    return (char*)it->entries[it->context.entryIndex].items[it->context.itemIndex].key.keyStr;
  }
  else
  {
    /* find the next used entry */
    for (entryIndex = it->context.entryIndex+1; entryIndex < it->size; entryIndex++)
    {
      if (it->entries[entryIndex].nextItemIndex > 0)
      {
        it->context.entryIndex = entryIndex;
        it->context.itemIndex = 0;
        return (char*)it->entries[entryIndex].items[0].key.keyStr;
      }
    }
  }

  return 0;
}

char *iupTableRemoveCurr(Itable *it)
{
  char* key;
  unsigned int entryIndex;
  ItableEntry  *entry;
  unsigned int itemIndex;

  iupASSERT(it!=NULL);
  if (!it || it->context.entryIndex == (unsigned int)-1
         || it->context.itemIndex == (unsigned int)-1)
    return 0;

  entry = &it->entries[it->context.entryIndex];
  itemIndex = it->context.itemIndex;

  if (it->context.itemIndex + 1 < it->entries[it->context.entryIndex].nextItemIndex)
  {
    /* key in the current entry */
    it->context.itemIndex++;
    key = (char*)it->entries[it->context.entryIndex].items[it->context.itemIndex].key.keyStr;

    iTableRemoveItem(it, entry, itemIndex);
    return key;
  }
  else
  {
    /* find the next used entry */
    for (entryIndex = it->context.entryIndex+1; entryIndex < it->size; entryIndex++)
    {
      if (it->entries[entryIndex].nextItemIndex > 0)
      {
        it->context.entryIndex = entryIndex;
        it->context.itemIndex = 0;
        key = (char*)it->entries[entryIndex].items[0].key.keyStr;

        iTableRemoveItem(it, entry, itemIndex);
        return key;
       }
    }
  }

  return 0;
}


/********************************************/
/*           Private functions              */
/********************************************/


static void iTableFreeItemArray(Itable_IndexTypes indexType, unsigned int nextFreeIndex, ItableItem *items)
{
  unsigned int i;

  /* Used only in iupTableClear */

  iupASSERT(items!=NULL);
  if (!items)
    return;

  if (indexType == IUPTABLE_STRINGINDEXED)
  {
    for (i = 0; i < nextFreeIndex; i++)
      free((void *)(items[i].key.keyStr));
  }

  for (i = 0; i < nextFreeIndex; i++)
  {
    if (items[i].itemType == IUPTABLE_STRING)
      free(items[i].value);
  }

  free(items);
}


static unsigned int iTableGetEntryIndex(Itable *it, const char *key, unsigned long *keyIndex)
{
  if (it->indexType == IUPTABLE_STRINGINDEXED)
  {
    register unsigned int checksum = 0;

/*     Orignal version
    unsigned int i;
    for (i = 0; key[i]; i++)
      checksum = checksum*31 + key[i];  
*/

    while (*key)
    {
      checksum *= 31;
      checksum += *key;
      key++;
    }

    *keyIndex = checksum;            /* this could NOT be dependent from table size */
  }
  else
  {
    /* Pointer indexed */
    *keyIndex = (unsigned long)key;   /* this could NOT be dependent from table size */
  }

  return (unsigned int)((*keyIndex) % it->size);
}

#ifdef DEBUGTABLE
static void iTableCheckDuplicated(ItableItem *item, unsigned int nextItemIndex, const char *key, 
                                                                          unsigned long keyIndex)
{
  unsigned int i;
  for (i = 0; i < nextItemIndex; i++, item++)
  {
    if (!iupStrEqual((char*)item->key.keyStr, (char*)key) && 
        item->key.keyIndex == keyIndex)
    {
      fprintf(stderr, "#ERROR# Duplicated key index (%ld): %s %s \n", keyIndex, 
                                                                     (char*)item->key.keyStr, 
                                                                     (char*)key);
    }
  }
}
#endif

static unsigned int iTableFindItem(Itable *it, const char *key, ItableEntry **entry, 
                                                          unsigned int *itemIndex,
                                                          unsigned long *keyIndex)
{
  unsigned int entryIndex,
               itemFound,
               i;
  ItableItem  *item;

  entryIndex = iTableGetEntryIndex(it, key, keyIndex);

  *entry = &(it->entries[entryIndex]);

  item = &((*entry)->items[0]);
  for (i = 0; i < (*entry)->nextItemIndex; i++, item++)
  {
    if (it->indexType == IUPTABLE_STRINGINDEXED)
      itemFound = item->key.keyIndex == *keyIndex; 
/*    itemFound = iupStrEqual(item->key.keyStr, key);  This is the original safe version */
    else
      itemFound = item->key.keyStr == key;

    if (itemFound)
    {
      *itemIndex = i;
      return 1;
    }
  }

  /* if not found "entry", "itemIndex" and "keyIndex" will have the new insert position. */

  *itemIndex = i;
  return 0;
}

static void iTableUpdateArraySize(ItableEntry *entry)
{
  if (entry->nextItemIndex >= entry->size)
  {
    /* we have to expand the item array */
    unsigned int newSize;

    newSize = entry->size + itable_itemGrow;

    entry->items = (ItableItem *)realloc(entry->items, newSize * sizeof(ItableItem));
    iupASSERT(entry->items!=NULL);
    if (!entry->items)
      return;

    memset(entry->items + entry->size, 0, itable_itemGrow * sizeof(ItableItem));

    entry->size = newSize;
  }
}


static void iTableAdd(Itable *it, ItableKey *key, void *value, Itable_Types itemType)
{
  unsigned int entryIndex;
  unsigned long keyIndex;
  ItableEntry *entry;
  ItableItem* item;

  entryIndex = iTableGetEntryIndex(it, key->keyStr, &keyIndex);

  entry = &(it->entries[entryIndex]);
  iTableUpdateArraySize(entry);

  /* add a new item at the end of the item array without duplicating memory. */
  item = &(entry->items[entry->nextItemIndex]);
  item->itemType     = itemType;
  item->key.keyIndex = keyIndex;
  item->key.keyStr   = key->keyStr;
  item->value        = value;

  entry->nextItemIndex++;
  it->numberOfEntries++;
}

static unsigned int iTableResize(Itable *it)
{
  unsigned int   newSizeIndex,
                 entryIndex,
                 i;
  Itable        *newTable;
  ItableEntry   *entry;
  ItableItem    *item;

  /* check if we do not need to resize the hash table */
  if (it->numberOfEntries == 0 ||
     it->tableSizeIndex >= itable_maxTableSizeIndex ||
     it->size / it->numberOfEntries >= itable_resizeLimit)
    return 0;

  /* create a new hash table and copy the contents of
   * the current table into the new one
   */
  newSizeIndex = it->tableSizeIndex + 1;
  newTable = iupTableCreateSized(it->indexType, newSizeIndex);

  for (entryIndex = 0; entryIndex < it->size; entryIndex++)
  {
    entry = &(it->entries[entryIndex]);

    if (entry->items)
    {
      item = &(entry->items[0]);

      for (i = 0; i < entry->nextItemIndex; i++, item++)
      {
        iTableAdd(newTable, &(item->key), item->value, item->itemType);
      }     

      free(entry->items);
    }
  }

  free(it->entries);

  it->size            = newTable->size;
  it->tableSizeIndex  = newTable->tableSizeIndex;
  it->numberOfEntries = newTable->numberOfEntries;
  it->entries         = newTable->entries;

  free(newTable);

  return 1;
}

#ifdef DEBUGTABLE
static void iTableShowStatistics(Itable *it)
{
  unsigned int nofSlots        = 0;
  unsigned int nofKeys         = 0;
  double optimalNofKeysPerSlot = 0.0;
  unsigned int nofSlotsWithMoreKeys = 0;
  unsigned int nofSlotsWithLessKeys = 0;

  unsigned int entryIndex;
  fprintf(stderr, "\n--- HASH TABLE STATISTICS ---\n");
  if (!it)
  {
    fprintf(stderr, "no hash table...\n");
    return;
  }

  nofSlots = it->size;
  nofKeys  = it->numberOfEntries;
  optimalNofKeysPerSlot = (double)nofKeys / (double)nofSlots;

  for (entryIndex = 0; entryIndex < it->size; entryIndex++)
  {
    ItableEntry *entry = &(it->entries[entryIndex]);

    if (entry->nextItemIndex > optimalNofKeysPerSlot + 3)
      nofSlotsWithMoreKeys++;
    else if (entry->nextItemIndex < optimalNofKeysPerSlot - 3)
      nofSlotsWithLessKeys++;
  }

  fprintf(stderr, "Number of slots: %d\n", nofSlots);
  fprintf(stderr, "Number of keys: %d\n", nofKeys);
  fprintf(stderr, "Optimal number of keys per slot: %f\n", optimalNofKeysPerSlot);
  fprintf(stderr, "Number of slots with much more keys: %d\n", nofSlotsWithMoreKeys);
  fprintf(stderr, "Number of slots with far less keys: %d\n", nofSlotsWithLessKeys);
  fprintf(stderr, "\n");
}
#endif
