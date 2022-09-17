/** @file TestSample.cpp
    @brief Sample Test class */
#include <stdio.h>
#include <string.h>
#include "TestBase.h"

using namespace test;

class TestSample : public TestBase {
public:
	void InitializeOnce(void) override;
	void FinalizeOnce(void) override;
	void InitializePerTest(void) override;
	void FinalizePerTest(void) override;
	bool RegisterTests(void) override;
	const char *OwnName(void) override;

	/** TestSample Constructor \n
        you must call @ref AddBaseQueue() to add your instance to internal queue @emoji :bulb:
	    @mscfile Init.msc "init sequence" */
	TestSample() { AddBaseQueue(this); }
	~TestSample() {}
};

namespace {
/** Instance of TestSample \n
    when DLL is loaded, this variable is activated (invoke constructor)
    @see TestSample::TestSample() */
TestSample Inst;

/** implement your test code(1.1.1) \n
    This test will not run unless you call test::TestBase::Register(). \n
	In this context, you can use following macro \n
    - TEST_LOG()
	- VERIFY()
	- VERIFY_STR()

	@param[in] This your instance
	@retval true test success
	@retval false test failed

	@mscfile Test2.msc "each test sequence" */
bool test_1_1_1(void *This)
{
	TestSample *Test = (TestSample *)This;

	VERIFY(5 == 5);
	//VERIFY(5 == 6);
	TEST_LOG("this is 1.1.1");

	return true;
}

/** same as 1.1.1 */
bool test_1_1_2(void *This)
{
	TestSample *Test = (TestSample *)This;

	TEST_LOG("this is 1.1.2");
	VERIFY_STR("hoge", "hoge");
	//VERIFY_STR("piyo", "hoge");

	return true;
}

/** same as 1.1.1 */
bool test_1_1_3(void *This)
{
	TestSample *Test = (TestSample *)This;

	TEST_LOG("this is 1.1.3");

	return true;
}

} // namespace

void TestSample::InitializeOnce(void)
{
	_TEST_LOG("START");
}

void TestSample::FinalizeOnce(void)
{
	_TEST_LOG("STOP");
}

void TestSample::InitializePerTest(void)
{
	_TEST_LOG("TEST START %s", GetTestID());
}

void TestSample::FinalizePerTest(void)
{
	_TEST_LOG("TEST STOP %s", GetTestID());
}

bool TestSample::RegisterTests(void)
{
	Register("1.1.1", test_1_1_1);
	Register("1.1.2", test_1_1_2);
	Register("1.1.3", test_1_1_3);
	return true;
}

const char *TestSample::OwnName(void)
{
	return "HOGE";
}

