PROJNAME = iup
LIBNAME  = iupluaim

IUP := ..

OPT = YES
# To not link with the Lua dynamic library in UNIX
NO_LUALINK = Yes
# To use a subfolder with the Lua version for binaries
LUAMOD_DIR = Yes

USE_IUP3 = Yes
USE_IUPLUA = Yes
USE_IMLUA = Yes

SRC = iuplua_im.c
DEF_FILE = iupluaim.def
INCLUDES = ../src
LIBS = iupim

ifdef USE_LUA53
  LUASFX = 53
else
ifdef USE_LUA52
  LUASFX = 52
else
  USE_LUA51 = Yes
  LUASFX = 51
endif
endif

LIBNAME := $(LIBNAME)$(LUASFX)

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  USE_IUPLUA:=
  USE_IMLUA:=
  USE_IM = Yes
endif
