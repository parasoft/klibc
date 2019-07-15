#
# (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
# The use of this source code is governed by the general Parasoft License Agreement.
# Redistribution strictly prohibited.
#

# Configuration for building static runtime library with klcc (wrapper around 
# gcc targeting klibc) compiler

# The following variables are used to compile the runtime source files:
CC:=klcc
CFLAGS:=-DCPPTEST_SHARED=0
CC_COMPILE_FLAG:=-c 
CC_OUT_FLAG:=-o 
OBJ_EXT:=o


# The following variables are used to create the runtime library:
LIBTOOL:=ar
LIBTOOL_FLAGS:=-ru
LIBTOOL_OUT_FLAG:=
LIB_PREFIX:=lib
LIB_EXT:=a


# The following variables are used to build the test executable:
LD:=klcc
LDFLAGS:=
LD_OUT_FLAG:=-o 
OUT_EXT:=out

