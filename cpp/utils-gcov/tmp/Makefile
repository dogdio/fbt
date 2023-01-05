WARNING    =
COVERAGE   =
OPTIMIZE   =
CXXFLAGS   = -fPIC
LDFLAGS    = -shared
INCLUDES   =

TARGET1    = hoge.so
TARGET2    = hoge2.so
TARGETS    = $(TARGET1) $(TARGET2)

all: $(TARGETS)

%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@

%.so: %.o
	@echo "  [LIB] $@"
	@$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *.so

