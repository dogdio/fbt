SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
MAKE  = make

WARNING    = -Wall -Wno-unused-value -Wno-unused -Wno-uninitialized
CXXFLAGS1 ?= $(WARNING) -O3 -std=c++11

FILES1    = $(shell find *.cpp)
DEPENDS1  = $(FILES1:.cpp=.d)
TARGETS   = hoge hoge_ex

all: $(TARGETS)

%.d: %.cpp
	$(CXX) $(CXXFLAGS1) -MM $< > $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS1) -c $<

hoge_ex.cpp: main.o
	../stubgen.pl -v -f func.txt -g hoge_ex.cpp main.o
	$(CXX) $(CXXFLAGS1) -MM $@ > hoge_ex.d

hoge: main.o Hoge.o
	$(CXX) $^ -o $@

hoge_ex: main.o hoge_ex.o
	$(CXX) $^ -o $@

-include $(DEPENDS1)

clean:
	rm -f $(TARGETS) *.[od] func.txt hoge_ex.cpp *.gch

