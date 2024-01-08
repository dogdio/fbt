SHELL ?= /bin/bash
CC    ?= gcc
CXX   ?= g++
LD    ?= g++
MAKE  ?= make

WARNING    = -Wall -Wno-unused-value -Wno-unused
CFLAGS    ?= $(WARNING) -O3
CXXFLAGS  ?= $(WARNING) -O3 -fPIC -std=c++11
LDFLAGS   ?= -shared
INCLUDES  ?= -I../interface -I../../utils/core

FILES1    = JobSample1.cpp JobSample2.cpp
FILES_ALL = $(FILES1)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1   = libUnder.a
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
	@echo "  [LIB] $@"
	@$(AR) rs $@ $(OBJS1)

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

