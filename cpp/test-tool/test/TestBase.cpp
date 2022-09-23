/** @file TestBase.cpp
    @brief Test framework base class */
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "TestBase.h"
using namespace test;
#include "TestBasePrivate.h"
#include "TestConfig.h"
#include "TestConfigName.h"

namespace {
	struct TestBasePrivate PrivateData;    /*!< test::TestBass private data */
	std::vector<TestBase *> VecInstances;  /*!< user Instance internal queue */
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
	config::ConfigIF *CIF = config::GetInstance(CONFIG_CATEGORY_GLOBAL);

	if((priv->LogFile == NULL) || (Format == NULL))
		return;
	
	va_start(args, Format);

	if(CIF->GetInteger(CONFIG_ADD_TIMESTAMP)) {
		char buf[TIME_BUF_SIZE];
		base_priv::GetTimeOfDay(buf);
		fprintf(priv->LogFile, "[%s]", buf);
	}
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

void GetTimeOfDay(char (&buf)[TIME_BUF_SIZE])
{
	struct timespec ts;
	struct tm tm;

	clock_gettime(CLOCK_REALTIME, &ts);
	if(localtime_r(&ts.tv_sec, &tm) != NULL) {
		snprintf(buf, TIME_BUF_SIZE, "%02d/%02d %02d:%02d:%02d.%03d",
			tm.tm_mon+1, tm.tm_mday, tm.tm_hour,
			tm.tm_min, tm.tm_sec, (int)(ts.tv_nsec / 1000000)); // nsec to msec
	}
}

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

