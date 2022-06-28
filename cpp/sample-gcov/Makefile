SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused
COVERAGE   = -fprofile-arcs -ftest-coverage -fno-inline
OPTIMIZE   = # -O3
CXXFLAGS  ?= $(WARNING) $(OPTIMIZE) -std=c++11 -fPIC -g $(COVERAGE)
CXXFLAGS2 ?= $(WARNING) $(OPTIMIZE) -std=c++11 -fPIC -g
LDFLAGS   ?= -shared
INCLUDES  ?= -I../test-tool/include

FILES1    = TestTarget.cpp
FILES2    = TestSample.cpp
FILES_ALL = $(FILES1) $(FILES2)

TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
OBJS2    := $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
OBJS_ALL := $(OBJS1) $(OBJS2)
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1   = libSampleGcov.so
TARGETS   = $(TARGET1)

all: $(TARGETS)

$(TMPDIR)/%.d: %.cpp
	@mkdir -p $(TMPDIR)
	@echo "  [MM] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s/\(.*:\)/$(TMPDIR)\/\1/" $@

# compile with gcov
$(OBJS1): $(TMPDIR)/%.o: %.cpp
	@echo "  [CXXg] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

# compile without gcov
$(OBJS2): $(TMPDIR)/%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS2) -c $< -o $@

$(TARGET1): $(OBJS_ALL)
	@echo "  [LIB] $@"
	@$(LD) $(LDFLAGS) $(OBJS_ALL) /usr/lib/gcc/x86_64-linux-gnu/10/libgcov.a -o $@
#	@$(LD) $(LDFLAGS) $(OBJS_ALL) -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

