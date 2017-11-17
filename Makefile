#CXX11_HOME = /home/alpha/opt/gcc

CXX11_HOME = /usr

ASIO_HOME = ../asio_alone

CXXFLAGS := \
 -g3 \
 -fPIC \
 -std=c++14 \
 -Wall \
 -Wextra \
 -gdwarf-2 \
 -gstrict-dwarf \
 -Wno-parentheses \
 -Wno-enum-compare \
 -Wdeprecated-declarations \
 -fmerge-all-constants  \
 -I $(CXX11_HOME)/include \
 -isystem $(ASIO_HOME) \
 -I.

RELEASE_FLAGS := \
 -O3 \
 -DNDEBUG \

DEBUG_FLAGS := \
 -O0 \
 -D_DEBUG \

LDFLAGS += \
 -static-libstdc++ -static-libgcc \
 -fmerge-all-constants \
 -L${CXX11_HOME}/lib64

LIBS := \
 -lpthread \
 -lrt


DIR := . arth arth/miniz rapidxml-1.13 ../kcp

SRC := $(foreach d, $(DIR), $(wildcard $(d)/*.cpp))
OBJS := $(patsubst %.cpp, %.o, $(SRC))
RELEASE_OBJ := $(patsubst %.o, %.ro, $(OBJS))
DEBUG_OBJ := $(patsubst %.o, %.do, $(OBJS))


CXX := export LD_LIBRARY_PATH=${CXX11_HOME}/lib; ${CXX11_HOME}/bin/g++

CC := export LD_LIBRARY_PATH=${CXX11_HOME}/lib; ${CXX11_HOME}/bin/gcc

DEPS:=$(patsubst %.o, %.d, $(OBJS))
.PHONY: all clean debug
all: reuseport
debug: reuseport_debug
-include $(DEPS)


%.ro : %.cpp
	$(CXX) -c $< $(CXXFLAGS) $(RELEASE_FLAGS) -o $@

%.do : %.cpp
	$(CXX) -c $< $(CXXFLAGS) $(DEBUG_FLAGS) -o $@

reuseport : $(RELEASE_OBJ)
	$(CXX) $^ -o $@.exe $(LDFLAGS) $(LIBS)

reuseport_debug : $(DEBUG_OBJ)
	$(CXX) $^ -o $@.exe $(LDFLAGS) $(LIBS)

clean:
	-rm -f $(DEP) $(RELEASE_OBJ) $(DEBUG_OBJ)
	-rm -f *.exe

