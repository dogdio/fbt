/** @file TestSample4Ex.cpp
    @brief
    - External library for TestSample4
    - Case of don't call test::TestBase::AddBaseQueue(), test framework only call dlopen
      - This module works independently of the test framework
    - LD_LIBRARY_PATH must be set if other libraries depend on this library

	@see TestSample4.cpp */
#include <stdio.h>
#include <string.h>

/** local class */
class Hoge
{
public:
	Hoge() { printf("init %s\n", __FUNCTION__); }
	~Hoge() { }
};

/** - static class variable
    - when call dlopen() libSample4.so
      - invoke Hoge constructor() before TestSample4 constructor(); */
Hoge h;

bool sample_strcmp(const char *a, const char *b)
{
	return strcmp(a, b) == 0 ? true : false;
}

