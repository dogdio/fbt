/** @file TestSample4.cpp
    @brief Sample Test class 4
    @attention
    If libSample4.so depend on libSample4Ex.so: \n
    -# When build libSample4.so, you must specify the external depends library (use -l option: -lSample4Ex) \n
    -# you must set Environment variable (please do the following) \n
    ```
    $ export LD_LIBRARY_PATH=./plugin2
    $ ./ExecTest -d ./plugin2
    ```
    @see TestSample4Ex.cpp */
#include <stdio.h>
#include <string.h>
#include "TestBase.h"

using namespace test;

class TestSample4 : public TestBase {
public:
	void InitializeOnce(void) override;
	void FinalizeOnce(void) override;
	void InitializePerTest(void) override;
	void FinalizePerTest(void) override;
	bool RegisterTests(void) override;
	const char *OwnName(void) override;

	TestSample4() { AddBaseQueue(this); printf("Sample4\n"); }
	~TestSample4() {}
};

extern bool sample_strcmp(const char *a, const char *b);

namespace {
TestSample4 Inst;

bool test_2_1_1(void *This)
{
	TestSample4 *Test = (TestSample4 *)This;

	VERIFY(5 == 5);
	//VERIFY(5 == 6);
	TEST_LOG("this is 2.1.1++++++++++++");

	return true;
}

bool test_2_1_2(void *This)
{
	TestSample4 *Test = (TestSample4 *)This;

	TEST_LOG("this is 2.1.2");
	VERIFY_STR("hoge", "hoge");
	//VERIFY_STR("piyo", "hoge");

	return true;
}

/** use libSample4.so function */
bool test_2_1_3(void *This)
{
	TestSample4 *Test = (TestSample4 *)This;

	VERIFY(sample_strcmp("abc", "def") == false);
	TEST_LOG("this is 2.1.3");

	return true;
}

} // namespace

void TestSample4::InitializeOnce(void)
{
	_TEST_LOG("START");
}

void TestSample4::FinalizeOnce(void)
{
	_TEST_LOG("STOP");
}

void TestSample4::InitializePerTest(void)
{
	_TEST_LOG("TEST START %s", GetTestID());
}

void TestSample4::FinalizePerTest(void)
{
	_TEST_LOG("TEST STOP %s", GetTestID());
}

bool TestSample4::RegisterTests(void)
{
	Register("2.1.1", test_2_1_1);
	Register("2.1.2", test_2_1_2);
	Register("2.1.3", test_2_1_3);
	return true;
}

const char *TestSample4::OwnName(void)
{
	return "HOGE4";
}

