#include <stdio.h>
#include <stdarg.h>

#include "TestBase.h"
using namespace test;
#include "TestBasePrivate.h"

namespace {
	struct TestBasePrivate PrivateData;
	std::vector<TestBase *> VecInstances;
};

//
// TestBase Common Function: for user code
//
TestBase::TestBase()
{
	PrivateData = {};
	priv = &PrivateData;
	priv->CurrentTestID = "";
}

TestBase::~TestBase()
{

}

const char *TestBase::GetTestID(void)
{
	return priv->CurrentTestID;
}

void TestBase::Logging(const char *Format, ...)
{
	va_list args;

	if((priv->LogFile == NULL) || (Format == NULL))
		return;
	
	va_start(args, Format);
	fprintf(priv->LogFile, "[%s] ", priv->CurrentTestID);
	vfprintf(priv->LogFile, Format, args);
	va_end(args);
}

bool TestBase::Register(const char *TestID, TEST_FUNC func)
{
	TEST_LIST tl;

	if(!priv->EnableRegister) {
		return false;
	}

	tl.TestID = TestID;
	tl.func = func;
	priv->TestList.push_back(tl);
	return true;
}

void TestBase::AddBaseQueue(TestBase *Base)
{
	VecInstances.push_back(Base);
}


//
// TestBase API: for TestMain
//
namespace test {
namespace base_priv {

void ClearQueue(void)
{
	VecInstances = {};
}

int ForeachQueue(DO_TEST func)
{
	int num = 0;

	if(func == NULL)
		goto end;

	for(auto Inst : VecInstances) {
		func(Inst);
		num++;
	}
end:
	return num;
}

} // base_priv
} // test

