SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
AR    = ar
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
ifeq ($(WITH_GCOV), 1)
 COVERAGE  = -fprofile-arcs -ftest-coverage -fno-inline
endif
CXXFLAGS  ?= $(WARNING) -O3 -std=c++11 $(COVERAGE) -fPIC -DWITH_GCOV=$(WITH_GCOV)
#CXXFLAGS  ?= $(WARNING) -g -std=c++11
LDFLAGS1   ?= -shared
INCLUDES  ?= -I.

FILES1    = $(wildcard *.cpp | grep -v Test)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.d))
LIBS      = -ldl -lpthread

TARGET1   = libUtils.so
TARGET2   = libUtils.a
TARGETS  = $(TARGET1) $(TARGET2)

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


