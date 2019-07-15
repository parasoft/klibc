#
# (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
# The use of this source code is governed by the general Parasoft License Agreement.
# Redistribution strictly prohibited.
#

# Configuration for building static runtime library targeting klibc 

# The following variables are used to compile the runtime source files:
KLIBC_PREFIX := ${HOME}/dev/cpptest/kernel_research/klibc_unpack/klibc-2.0.5/usr
KLIBC_ARCH := x86_64
KLIBC_BITNESS := 64
CC:=gcc
CFLAGS:=-DCPPTEST_SHARED=0 \
	-D__KLIBC__=2 \
	-D__KLIBC_MINOR__=0 \
	-D_BITSIZE=${KLIBC_BITNESS} \
	-fno-stack-protector \
	-fwrapv \
	-m64 \
	-nostdinc \
	-iwithprefix include \
	-I $(KLIBC_PREFIX)/include/arch/$(KLIBC_ARCH) \
	-I $(KLIBC_PREFIX)/include/bits${KLIBC_BITNESS} \
	-I $(KLIBC_PREFIX)/include

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
LDFLAGS:= -v 
LD_OUT_FLAG:=-o 
OUT_EXT:=out

