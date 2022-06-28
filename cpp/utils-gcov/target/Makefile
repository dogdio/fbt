
WARNING    = -Wall -Wno-unused-value -Wno-unused
COVERAGE   = -fprofile-arcs -ftest-coverage -fno-inline #-fno-exceptions
CXXFLAGS  ?= $(WARNING) -std=c++11 -fPIC -g $(COVERAGE)
INCLUDES  ?= -I../test-tool/include

FILES1    = $(wildcard *.cpp)
TMPDIR    = .objs
OBJS1    := $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.o))
DEPENDS1  = $(addprefix $(TMPDIR)/, $(FILES1:.cpp=.d))

TARGETS   = $(OBJS1)

all: $(TARGETS)

$(TMPDIR)/%.d: %.cpp
	@mkdir -p $(TMPDIR)
	@echo "  [MM] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s/\(.*:\)/$(TMPDIR)\/\1/" $@

$(TMPDIR)/%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

-include $(DEPENDS1)

clean:
	rm -rf $(TMPDIR)
	rm -f $(TARGETS)


