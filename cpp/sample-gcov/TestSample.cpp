#include <stdio.h>
#include "TestTarget.h"
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

	TestSample() { AddBaseQueue(this); }
	~TestSample() {}
};

namespace {
TestSample Inst;

bool test_1_1_1(void *This)
{
	VERIFY(get_true() == true);
	return true;
}

bool test_1_2_1(void *This)
{
	VERIFY(is_zero(0) == true);
	return true;
}

bool test_1_2_2(void *This)
{
	VERIFY(is_zero(5) == false);
	return true;
}

bool test_1_3_1(void *This)
{
	VERIFY(is_zero_zero(0, 0) == true);
	VERIFY(is_zero_zero_2(0, 0) == true);
	VERIFY(is_zero_zero_3(0, 0) == true);
	VERIFY(is_zero_zero_4(0, 0) == true);
	TEST_LOG("normal route OK");
	return true;
}

bool test_1_3_2(void *This)
{
	// この試験まで実施したとき
	// branch coverage=0の場合、zero_zero(), zero_zero_2()のカバレッジは100%となる
	// branch coverage=1の場合、4つの関数はまだ100%にならない
	VERIFY(is_zero_zero(0, 5) == false);
	VERIFY(is_zero_zero_2(0, 5) == false);
	VERIFY(is_zero_zero_3(0, 5) == false);
	VERIFY(is_zero_zero_4(0, 5) == false);
	TEST_LOG("illegal route1 OK");
	return true;
}

bool test_1_3_3(void *This)
{
	// branch coverage=1の場合、この試験まで実施すると4つとも100%となる
	// 引数が両方とも0でないケース(5, 5) も本来はやるべき(この試験対象の関数では無意味なだけ)
	VERIFY(is_zero_zero(5, 0) == false);
	VERIFY(is_zero_zero_2(5, 0) == false);
	VERIFY(is_zero_zero_3(5, 0) == false);
	VERIFY(is_zero_zero_4(5, 0) == false);
	TEST_LOG("illegal route2 OK");
	return true;
}

} // anonymous namespace

void TestSample::InitializeOnce(void)
{
	_TEST_LOG("START Once");
}

void TestSample::FinalizeOnce(void)
{
	_TEST_LOG("STOP Once");
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
	Register("1.2.1", test_1_2_1);
	Register("1.2.2", test_1_2_2);

	Register("1.3.1", test_1_3_1);
	Register("1.3.2", test_1_3_2);
	Register("1.3.3", test_1_3_3);

	return true;
}

const char *TestSample::OwnName(void)
{
	return "SampleTest";
}

