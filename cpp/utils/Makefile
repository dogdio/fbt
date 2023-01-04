SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
AR    = ar
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
CXXFLAGS  ?= $(WARNING) -O3 -std=c++11 -fPIC
#CXXFLAGS  ?= $(WARNING) -g -std=c++11
LDFLAGS1   ?= -shared
LDFLAGS3  ?= -rdynamic
INCLUDES  ?= -I.

FILES1    = $(wildcard *.cpp | grep -v Sample)
FILES3    = QueueSample.cpp
TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
OBJS3    := $(addprefix $(TMPDIR)/, $(FILES3:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.d))
LIBS      = -ldl -lpthread

TARGET1   = libUtils.so
TARGET2   = libUtils.a
TARGET3   = QueueSample
TARGETS   = $(TARGET1) $(TARGET2) $(TARGET3)

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

$(TARGET3): $(OBJS3) $(TARGET2)
	@echo "  [EXE] $@"
	@$(LD) -o $@ $(OBJS3) $(LDFLAGS3) $(TARGET2) $(LIBS)

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)


