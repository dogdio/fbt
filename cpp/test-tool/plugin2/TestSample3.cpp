/** @file TestSample3.cpp
    @brief Sample Test class 3 */
#include <stdio.h>
#include <string.h>
#include "TestBase.h"

using namespace test;

class TestSample3 : public TestBase {
public:
	void InitializeOnce(void) override;
	void FinalizeOnce(void) override;
	void InitializePerTest(void) override;
	void FinalizePerTest(void) override;
	bool RegisterTests(void) override;
	const char *OwnName(void) override;

	/** TestSample3 Constructor \n
        you must call @ref AddBaseQueue() to add your instance to internal queue @emoji :bulb:
	    @mscfile Init.msc "init sequence" */
	TestSample3() { AddBaseQueue(this); }
	~TestSample3() {}
};

namespace {
/** Instance of TestSample3 \n
    when DLL is loaded, this variable is activated (invoke constructor)
    @see TestSample3::TestSample3() */
TestSample3 Inst;

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
	TestSample3 *Test = (TestSample3 *)This;

	VERIFY(5 == 4);
	//VERIFY(5 == 6);
	TEST_LOG("this is 1.1.1");

	return true;
}

/** same as 1.1.1 */
bool test_1_1_2(void *This)
{
	TestSample3 *Test = (TestSample3 *)This;

	TEST_LOG("this is 1.1.2");
	VERIFY_STR("hoge", "hoge");
	//VERIFY_STR("piyo", "hoge");

	return true;
}

/** same as 1.1.1 */
bool test_1_1_3(void *This)
{
	TestSample3 *Test = (TestSample3 *)This;

	TEST_LOG("this is 1.1.3");

	return true;
}

} // namespace

void TestSample3::InitializeOnce(void)
{
	_TEST_LOG("START");
}

void TestSample3::FinalizeOnce(void)
{
	_TEST_LOG("STOP");
}

void TestSample3::InitializePerTest(void)
{
	_TEST_LOG("TEST START %s", GetTestID());
}

void TestSample3::FinalizePerTest(void)
{
	_TEST_LOG("TEST STOP %s", GetTestID());
}

bool TestSample3::RegisterTests(void)
{
	Register("1.1.1", test_1_1_1);
	Register("1.1.2", test_1_1_2);
	Register("1.1.3", test_1_1_3);
	return true;
}

const char *TestSample3::OwnName(void)
{
	return "HOGE3";
}

