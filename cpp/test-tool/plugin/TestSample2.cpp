#include <stdio.h>
#include <string.h>
#include "TestBase.h"

using namespace test;

class TestSample2 : public TestBase {
public:
	void InitializeOnce(void) override;
	void FinalizeOnce(void) override;
	void InitializePerTest(void) override;
	void FinalizePerTest(void) override;
	bool RegisterTests(void) override;
	const char *OwnName(void) override;

	TestSample2() { AddBaseQueue(this); }
	~TestSample2() {}
};

namespace {
TestSample2 Inst;

bool test_2_1_1(void *This)
{
	TestSample2 *Test = (TestSample2 *)This;

	VERIFY(5 == 5);
	//VERIFY(5 == 6);
	TEST_LOG("this is 2.1.1++++++++++++");

	return true;
}

bool test_2_1_2(void *This)
{
	TestSample2 *Test = (TestSample2 *)This;

	TEST_LOG("this is 2.1.2");
	VERIFY_STR("hoge", "hoge");
	//VERIFY_STR("piyo", "hoge");

	return true;
}

bool test_2_1_3(void *This)
{
	TestSample2 *Test = (TestSample2 *)This;

	TEST_LOG("this is 2.1.3");

	return true;
}

} // namespace

void TestSample2::InitializeOnce(void)
{
	_TEST_LOG("START");
}

void TestSample2::FinalizeOnce(void)
{
	_TEST_LOG("STOP");
}

void TestSample2::InitializePerTest(void)
{
	_TEST_LOG("TEST START %s", GetTestID());
}

void TestSample2::FinalizePerTest(void)
{
	_TEST_LOG("TEST STOP %s", GetTestID());
}

bool TestSample2::RegisterTests(void)
{
	Register("2.1.1", test_2_1_1);
	Register("2.1.2", test_2_1_2);
	Register("2.1.3", test_2_1_3);
	return true;
}

const char *TestSample2::OwnName(void)
{
	return "HOGE2";
}

