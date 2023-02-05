SHELL ?= /bin/bash
CC    ?= gcc
CXX   ?= g++
LD    ?= g++
MAKE  ?= make

WARNING    = -Wall -Wno-unused-value -Wno-unused
CFLAGS    ?= $(WARNING) -O3
CXXFLAGS  ?= $(WARNING) -O3 -fPIC -std=c++11
LDFLAGS   ?= -shared
INCLUDES  ?= -I../interface -I../../utils

FILES1    = JobSample.cpp
FILES2    = JobSample2.cpp
FILES_ALL = $(FILES1) $(FILES2)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
OBJS2    := $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
OBJS_ALL := $(OBJS1) $(OBJS2)
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1   = Sample.so
TARGET2   = Sample2.so
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
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@

$(TARGET2): $(OBJS2)
	@echo "  [LIB] $@"
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

