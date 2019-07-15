#
# (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
# The use of this source code is governed by the general Parasoft License Agreement.
# Redistribution strictly prohibited.
#

# Configuration for building dynamic runtime library with gcc compiler

# The following variables are used to compile the runtime source files:
CC:=klcc
CFLAGS:=-fPIC -DCPPTEST_SHARED=1
CC_COMPILE_FLAG:=-c 
CC_OUT_FLAG:=-o 
OBJ_EXT:=o


# The following variables are used to create the runtime library:
LIBTOOL:=klcc
LIBTOOL_FLAGS:=-shared
LIBTOOL_OUT_FLAG:=-o 
LIB_PREFIX:=lib
LIB_EXT:=so


# The following variables are used to build the test executable:
LD:=klcc
LDFLAGS:=-shared
LD_OUT_FLAG:=-o 
OUT_EXT:=out

