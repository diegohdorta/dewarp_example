# Copyright 2020 NXP Semiconductors
# SPDX-License-Identifier: BSD-3-Clause

APPNAME		= run
DESTDIR		= media
SRCDIR		= dewarp.cpp dewarp.hpp

ROOTFS_DIR = $(SDKTARGETSYSROOT)

TARGET_PATH_LIB 	= $(ROOTFS_DIR)/usr/lib
TARGET_PATH_INCLUDE 	= $(ROOTFS_DIR)/usr/include
COMMON_DIR		= helper

CFLAGS	= -DLINUX  \
	  -g3 \
	  -Ofast \
	  -fsigned-char \
	  -Wno-attributes \
	  -Wno-strict-aliasing \
	  -Wno-comment \
	  -Wall \
	  -Wextra \
	  -Werror \
	  -Wno-unused-parameter \
	  -Wno-missing-field-initializers \
	  -Wno-format-security \
	  -Waggregate-return \
	  -Wcast-qual \
	  -Wunreachable-code \
	  -fvisibility=hidden \
	  -Wfloat-equal \
	  -Wundef \
	  -Wshadow \
	  -Wpointer-arith \
	  -fsanitize=leak \
	  -fno-omit-frame-pointer \
	  -DEGL_API_FB \
	  -DEGL_API_WL \
	  -I. -I$(TARGET_PATH_INCLUDE) \
	  -I$(COMMON_DIR)/inc \
          -I$(TARGET_PATH_INCLUDE)/glib-2.0 \
          -I$(TARGET_PATH_LIB)/glib-2.0/include \
                        

LFLAGS	= -std=c++14 \
	  -Wl,--library-path=$(TARGET_PATH_LIB),-rpath-link=$(TARGET_PATH_LIB) \
	  -lm -Wno-attributes \
	  -lGAL \
	  -lEGL \
	  -lGLESv2 \
	  -lIL \
	  -ldl \
	  -lwayland-client \
	  -lwayland-cursor \
	  -lwayland-egl 

OBJECTS	= dewarp.o $(COMMON_DIR)/src/fsl_egl.o 

first: all

all: $(APPNAME)

$(APPNAME): $(OBJECTS) 
	$(CXX) $(LFLAGS) -o $(DESTDIR)/$(APPNAME) $(OBJECTS)  

dewarp.o: dewarp.cpp dewarp.hpp
	$(CXX) $(CFLAGS)  -c -o dewarp.o dewarp.cpp 

fsl_egl.o: fsl_egl.c
	$(CXX) $(CFLAGS) -c -o fsl_egl.o fsl_egl.c


