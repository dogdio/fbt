SHELL ?= /bin/bash
CC    ?= gcc
CXX   ?= g++
LD    ?= g++
MAKE  ?= make

WARNING    = -Wall -Wno-unused-value -Wno-unused
CFLAGS    ?= $(WARNING) -O3
CXXFLAGS  ?= $(WARNING) -O3 -fPIC -std=c++11
LDFLAGS   ?= -shared
INCLUDES  ?= -I../interface

FILES1    = InstSample1.cpp
FILES2    = InstSample2.cpp
FILES3    = InstSample3.cpp
FILES_ALL = $(FILES1) $(FILES2) $(FILES3)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
OBJS2    := $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
OBJS3    := $(addprefix $(TMPDIR)/, $(FILES3:.cpp=.o))
OBJS_ALL := $(OBJS1) $(OBJS2) $(OBJS3)
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1   = Sample1.so
TARGET2   = Sample2.so
TARGET3   = Sample3.so
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
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@

$(TARGET2): $(OBJS2)
	@echo "  [LIB] $@"
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@

$(TARGET3): $(OBJS3)
	@echo "  [LIB] $@"
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

