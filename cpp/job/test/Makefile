
WARNING    = -Wall -Wno-unused-value -Wno-unused
#COVERAGE   = -fprofile-arcs -ftest-coverage -fno-inline #-fno-exceptions
OPTIMIZE   = # -O3
CXXFLAGS   = $(WARNING) $(OPTIMIZE) -std=c++11 -fPIC -g
LDFLAGS    = -shared
INCLUDES   = -I../../test-tool/include -I../export -I../interface -I../../utils

FILES2     = TestSample.cpp
FILES_ALL  = $(FILES2)
TMPDIR     = .objs

OBJ_DIR    = ../core
OBJS1      = $(shell find $(OBJ_DIR) -name "*.o")
OBJS2      = $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
LIBS       = -lgcov ../../utils/libUtils.a

DEPENDS1   = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))

TARGET1    = libJobTest.so
TARGET2    = 
TARGETS    = $(TARGET1) $(TARGET2)

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
	@$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)

