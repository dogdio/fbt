SHELL ?= /bin/bash
CC    ?= gcc
CXX   ?= g++
LD    ?= g++
MAKE  ?= make

WARNING    = -Wall -Wno-unused-value -Wno-unused
CFLAGS    ?= $(WARNING) -O3
CXXFLAGS  ?= $(WARNING) -O3 -fPIC -std=c++11
LDFLAGS   ?= -shared
INCLUDES  ?= -I../include

FILES1    = TestSample3.cpp
FILES2    = TestSample4.cpp
FILES3    = TestSample4Ex.cpp
FILES_ALL = $(FILES1) $(FILES2) $(FILES3)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
OBJS2    := $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
OBJS3    := $(addprefix $(TMPDIR)/, $(FILES3:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1   = libSample3.so
TARGET2   = libSample4.so
TARGET3   = libSample4Ex.so
TARGETS   = $(TARGET1) $(TARGET3) $(TARGET2)

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
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@ -L./ -lSample4Ex

$(TARGET3): $(OBJS3)
	@echo "  [LIB] $@"
	@$(CXX) $(INCLUDES) $(LDFLAGS) $< -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

