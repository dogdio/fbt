
WARNING    = -Wall -Wno-unused-value -Wno-unused
COVERAGE   = -fprofile-arcs -ftest-coverage -fno-inline #-fno-exceptions
CXXFLAGS  ?= $(WARNING) -std=c++11 -fPIC -g $(COVERAGE)
INCLUDES  ?= -I../test-tool/include

FILES1    = $(wildcard *.cpp)
OBJS1    := $(FILES1:.cpp=.o)
DEPENDS1  = $(FILES1:.cpp=.d)

TARGETS   = $(OBJS1)

all: $(TARGETS)

%.d: %.cpp
	@echo "  [MM] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $< > $@

%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

-include $(DEPENDS1)

clean:
	rm -f $(TARGETS) *.o *.d *.gcno *.gcda


