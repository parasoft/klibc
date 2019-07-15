#
# (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
# The use of this source code is governed by the general Parasoft License Agreement.
# Redistribution strictly prohibited.
#

# This is the main C++test runtime library makefile



#####################################################
#                                                   #
#                Common settings                    #
#                                                   #
#####################################################

# Choose target configuration (appropriate file will be included):
TARGET_DIR:=target
#TARGET_CFG:=gcc-static.mk
#TARGET_CFG:=gcc-shared.mk
#TARGET_CFG:=klcc-static.mk
#TARGET_CFG:=klcc-shared.mk
TARGET_CFG:=klibc-static.mk

# Choose type of communication between runtime library and C++test:
CHANNEL_DIR:=channel
CHANNEL_TYPE:=default

# Specify C++test includes directory:
CPPTEST_INC_DIR:=include
RUNTIME_INC_DIR:=src

# Specify the output directory for runtime library:
OUT_DIR:=build

# Specify the output directory for object files:
OBJ_DIR:=$(OUT_DIR)/objs

#####################################################
#                                                   #
#                 Library name                      #
#                                                   #
#####################################################

# Specify the name of runtime library:
LIB_NAME=cpptestruntime


#####################################################
#                                                   #
#              Target configuration                 #
#                                                   #
#####################################################

# Target configuration should define the following variables:
#
# PLATFORM, CC, CFLAGS, CC_COMPILE_FLAG, CC_OUT_FLAG, OBJ_EXT
# LIBTOOL, LIBTOOL_FLAGS, LIBTOOL_OUT_FLAG, LIB_PREFIX, LIB_EXT
# LD, LDFLAGS, LD_OUT_FLAG, OUT_EXT

TARGET_FILE:=$(TARGET_DIR)/$(TARGET_CFG)

# Including target config:
include $(TARGET_FILE)

#####################################################
#                                                   #
#             Communication channel                 #
#                                                   #
#####################################################

# Communication channel configuration should define the following variables:
#
# CHANNEL_CONFIG_MACRO

CHANNEL_FILE=$(CHANNEL_DIR)/$(CHANNEL_TYPE).mk

# Including communication channel configuration
include $(CHANNEL_FILE)

#####################################################
#                                                   #
#              Include dirs                         #
#                                                   #
#####################################################

ifdef HEWOPTIONSHANDLING
INCLUDES:="-I=$(RUNTIME_INC_DIR),$(CPPTEST_INC_DIR)"
else
INCLUDES:="-I$(RUNTIME_INC_DIR)" "-I$(CPPTEST_INC_DIR)"
endif

#####################################################
#                                                   #
#              Sources and objects                  #
#                                                   #
#####################################################

RUNTIME_SRCS:=$(wildcard src/*.c src/common/*.c src/presentation/*.c src/transport/*.c)
TEST_SRCS:=$(wildcard tests/src/*.c)

RUNTIME_OBJS:=$(RUNTIME_SRCS:%.c=$(OBJ_DIR)/%.$(OBJ_EXT))
TEST_OBJS:=$(TEST_SRCS:%.c=$(OBJ_DIR)/%.$(OBJ_EXT))

ALL_OBJS:=$(RUNTIME_OBJS)

ifdef FR_SOFTUNE_OPTIONSHANDLING
LIBTOOL_OBJS:=$(foreach var, $(ALL_OBJS), $(LIBTOOL_INPUT_FLAG) $(var))
else
LIBTOOL_OBJS:=$(ALL_OBJS)
endif

ALL_OBJ_DIRS:=$(OBJ_DIR)/src/common \
              $(OBJ_DIR)/src/presentation \
              $(OBJ_DIR)/src/transport \
              $(OBJ_DIR)/src \
			  $(OBJ_DIR)/tests/src

#####################################################
#                                                   #
#                Test executable                    #
#                                                   #
#####################################################

TEST_EXE:=$(OUT_DIR)/test.$(OUT_EXT)

#####################################################
#                                                   #
#               Library file nameble                #
#                                                   #
#####################################################

LIB_FILE_NAME:=$(LIB_PREFIX)$(LIB_NAME).$(LIB_EXT)

#####################################################
#                                                   #
#                 Makefile rules                    #
#                                                   #
#####################################################

.PHONY: all clean test

# Default goal
all: $(ALL_OBJ_DIRS) $(OUT_DIR)/$(LIB_FILE_NAME) $(TARGET_SPECIAL_GOAL)

# # Rule for object's directories
$(ALL_OBJ_DIRS):
	mkdir -p $@

# Rule for object building:
ifdef CC_OUT_DIR_FLAG  # this one is for compilers that don't support direct output file specification
$(OBJ_DIR)/%.$(OBJ_EXT): %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CHANNEL_CONFIG_MACRO) $(INCLUDES) $(CC_COMPILE_FLAG)"$<" $(CC_OUT_DIR_FLAG)"$(dir $@)"
else                   # this (regular) one is for the rest of compilers
$(OBJ_DIR)/%.$(OBJ_EXT): %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CHANNEL_CONFIG_MACRO) $(INCLUDES) $(CC_COMPILE_FLAG)"$<" $(CC_OUT_FLAG)"$@"
endif

# Build library:
%.$(LIB_EXT): $(ALL_OBJS)
	mkdir -p $(OUT_DIR)
	$(LIBTOOL) $(LIBTOOL_FLAGS) $(EXTRA_LIBTOOL_FLAGS) $(LIBTOOL_OUT_FLAG)"$@" $(LIBTOOL_OBJS)       

test: $(TEST_EXE)
	$(TEST_EXE)

$(TEST_EXE): $(ALL_OBJ_DIRS) $(OUT_DIR)/$(LIB_FILE_NAME) $(TEST_OBJS)
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) $(TEST_OBJS) $(OUT_DIR)/$(LIB_FILE_NAME) $(LD_OUT_FLAG)"$(TEST_EXE)"

# Clean the runtime library
clean:
	rm -f $(OUT_DIR)/$(LIB_FILE_NAME)      
	rm -f $(OUT_DIR)/test.$(OUT_EXT)
	rm -rf $(ALL_OBJ_DIRS)

