
WARNING    = -Wall -Wno-unused-value -Wno-unused
COVERAGE   = -fprofile-arcs -ftest-coverage -fno-inline #-fno-exceptions
OPTIMIZE   = # -O3
CXXFLAGS  ?= $(WARNING) $(OPTIMIZE) -std=c++11 -fPIC -g
LDFLAGS   ?= -shared
INCLUDES  ?= -I../../test-tool/include -I../target

FILES2    = TestSample.cpp
FILES3    = TestSample2.cpp
FILES_ALL = $(FILES2) $(FILES3)
TMPDIR    = .objs

SYM_DIR   = ../target
OBJS1    := $(shell find $(SYM_DIR) -name "*.o")

OBJS2    := $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
OBJS3    := $(addprefix $(TMPDIR)/, $(FILES3:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1   = libUtilsGcov.so  # etc..
TARGET2   = libUtilsGcov2.so # Log/Config
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

$(TARGET1): $(OBJS1) $(OBJS2)
	@echo "  [LIB] $@"
	@$(LD) $(LDFLAGS) $^ /usr/lib/gcc/x86_64-linux-gnu/10/libgcov.a -o $@

$(TARGET2): $(OBJS1) $(OBJS3)
	@echo "  [LIB] $@"
	@$(LD) $(LDFLAGS) $^ /usr/lib/gcc/x86_64-linux-gnu/10/libgcov.a -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

