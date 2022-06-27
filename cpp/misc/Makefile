SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
OPT        = -fno-exceptions
CXXFLAGS1 ?= $(WARNING) -O3 -std=c++11
CXXFLAGS2 ?= $(WARNING) -O3 -std=c++11 $(OPT)

FILES1    = $(shell find *.cpp | grep -v no_exception)
FILES2    = $(shell find *.cpp | grep no_exception)
TARGET1   = $(FILES1:.cpp=)
TARGET2   = $(FILES2:.cpp=)
TARGETS   = $(TARGET1) $(TARGET2)
LIB1      = -lpthread

all: $(TARGETS)

# TARGETによってCompile Optionを切り替える
$(TARGET1): %: %.cpp
	@echo "  [CXX] $^"
	@$(CXX) $(CXXFLAGS1) $^ -o $@ $(LDFLAGS) $(LIB1)

$(TARGET2): %: %.cpp
	@echo "  [CXX2] $^"
	@$(CXX) $(CXXFLAGS2) $^ -o $@ $(LDFLAGS) $(LIB1)

clean:
	rm -f $(TARGETS)


