SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
AR    = ar
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
#WARNING    = -Wall
ifeq ($(WITH_GCOV), 1)
 COVERAGE  = -fprofile-arcs -ftest-coverage -fno-inline
endif
CFLAGS    ?= $(WARNING) -O3
CXXFLAGS  ?= $(WARNING) -O3 -std=c++11 $(COVERAGE) -fPIC
LDFLAGS1  ?= -shared
INCLUDES  ?= -I../interface -I../export -I. -I../../utils

FILES1    = $(wildcard *.cpp)
TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.d))

TARGET1   = libJob.so
TARGET2   = libJob.a
TARGETS   = $(TARGET1) $(TARGET2)

all: $(TARGETS)

$(TMPDIR)/%.d: %.cpp
	@mkdir -p $(TMPDIR)
	@echo "  [MM] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s/\(.*:\)/$(TMPDIR)\/\1/" $@

$(TMPDIR)/%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

$(TARGET1): $(OBJS1)
	@echo "  [LIB] $@"
	@$(LD) $(LDFLAGS1) $(OBJS1) -o $@

$(TARGET2): $(OBJS1)
	@echo "  [LIB] $@"
	@$(AR) rs $@ $(OBJS1)

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

