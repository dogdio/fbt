SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
#WARNING    = -Wall
#COVERAGE   = -fprofile-arcs -ftest-coverage -fno-inline
CFLAGS    ?= $(WARNING) -O3
CXXFLAGS  ?= $(WARNING) -O3 -std=c++11 $(COVERAGE)
LDFLAGS1  ?= -rdynamic
LDFLAGS2  ?= -shared
INCLUDES  ?= -I../utils/core -I./interface -I./core

FILES1    = $(wildcard *.cpp)
TMPDIR    = .objs
#OBJS1    := $(FILES1:.cpp=.o)
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.d))
LIB1      =  ../utils/core/libUtils.a

TARGET1   = ./ccat
TARGETS   = $(TARGET1)

all: $(TARGETS)

$(TMPDIR)/%.d: %.cpp
	@mkdir -p $(TMPDIR)
	@echo "  [MM] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s/\(.*:\)/$(TMPDIR)\/\1/" $@

$(TMPDIR)/%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

$(TARGET1): $(OBJS1) $(LIB1)
	@echo "  [EXE] $@"
	@$(LD) -o $@ $(OBJS1) $(LDFLAGS1) $(LIB1)


-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

