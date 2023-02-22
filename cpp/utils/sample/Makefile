SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
AR    = ar
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
CXXFLAGS  ?= $(WARNING) -O3 -std=c++11 -fPIC
#CXXFLAGS  ?= $(WARNING) -g -std=c++11
#LDFLAGS1   ?= -shared
LDFLAGS1  ?= -rdynamic
INCLUDES  ?= -I../core

FILES1    = QueueSample.cpp
FILES_ALL = $(FILES1)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))
LIBS      = -ldl -lpthread

TARGET1   = QueueSample
TARGET2   =
TARGET3   =
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

$(TARGET1): $(OBJS1)
	@echo "  [EXE] $@"
	@$(LD) -o $@ $(OBJS1) $(LDFLAGS1) $(LIBS)

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)


