###############################################################
#
# Purpose: Makefile for compiling a controller using the Webots
#          API on the ranger real robot
# Author : david.mansolino@epfl.ch
# Date   : December 2014
#
###############################################################

# -------------------------------------------------------------


# name of the binary to generate
TARGET = controller

# Depedency libraries
DASHEL_LIB = $(ASEBA_BASE)/build-dashel/libdashel.so
ASEBA_LIB =  $(ASEBA_BASE)/build-aseba/common/libasebacommon.a
ASEBA_COMPILER_LIB =  $(ASEBA_BASE)/build-aseba/compiler/libasebacompiler.a
ASEBA_DASHEL_PLUGIN_LIB =  $(ASEBA_BASE)/build-aseba/transport/dashel_plugins/libasebadashelplugins.a

# pathes
WEBOTS_RANGER_LIB = $(RANGER_BASICS_BASE)/ranger_lib/lib/rangerWrapper.a
WEBOTS_RANGER_INCLUDE = $(RANGER_BASICS_BASE)/ranger_lib/include

# source filenames
CXX_SOURCES = \
  src/main.cpp \
  src/Ranger.cpp

# -------------------------------------------------------------



INCLUDE_DIRS = -I$(WEBOTS_RANGER_INCLUDE) -I./include
CXX = g++
CXXFLAGS += -O2 -DLINUX -DCROSSCOMPILATION -Wall $(INCLUDE_DIRS) 
LFLAGS += 
LIBS = $(WEBOTS_RANGER_LIB) $(DASHEL_LIB) $(ASEBA_COMPILER_LIB) $(ASEBA_DASHEL_PLUGIN_LIB) $(ASEBA_LIB) -ludev -lpthread
OBJECTS = $(CXX_SOURCES:.cpp=.o)

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET):$(OBJECTS)
	$(CXX) $(CFLAGS) $(LFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET) $(CXXFLAGS)
	chmod 755 $(TARGET)

