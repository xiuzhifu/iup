
/* avoid multiple inclusions */
#ifndef SIMPLE_PAINT_UTIL_H
#define SIMPLE_PAINT_UTIL_H

#include <im.h>
#include <im_image.h>


char* str_duplicate(const char* str);
const char* str_filetitle(const char *filename);
const char* str_fileext(const char *filename);
int str_compare(const char *l, const char *r, int casesensitive);

void show_error(const char* message, int is_error);
void show_file_error(int error);

void image_flood_fill(const imImage* image, int start_x, int start_y, long replace_color, double tol_percent);

#endif