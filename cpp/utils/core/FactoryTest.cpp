#include <stdint.h>

#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"
#include "FactoryTest.h"

using namespace Utils;

namespace {
bool NameVerify = false;
FactoryTest::Result TestResult;

class Test_IF {
public:
	Test_IF() {}
	virtual ~Test_IF() {}
};

class Test_Impl : public Test_IF {
public:
	~Test_Impl() {}
	Test_Impl(const char *Name) {
		if(NameVerify) {
			if(Name == NULL)
				throw std::invalid_argument("Name is null");
		}
	}
};

Factory::FactoryIF<Test_Impl, Test_IF> Inst("Factory.h.Test");

}

namespace Utils {
namespace FactoryTest {

Result &CreateFail(void)
{
	TestResult = {};

	NameVerify = false;
	TestResult.Fail1 = Inst.Create(NULL);
	NameVerify = true;
	TestResult.Fail2 = Inst.Create(NULL);

	TestResult.GetInstance = Inst.GetInstance("tmp");
	TestResult.Destroy = Inst.Destroy(NULL);

	return TestResult;
}

Result &CreateSuccess(void)
{
	TestResult = {};

	NameVerify = true;
	TestResult.Create = Inst.Create("tmp");
	TestResult.GetInstance = Inst.GetInstance("tmp");
	TestResult.Fail1 = Inst.Create("tmp");
	TestResult.Destroy = Inst.Destroy("tmp");

	return TestResult;
}

}
}
