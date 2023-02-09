#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>

#include "TestBase.h"
#include "Config.h"
#define MY_LOG_TYPE Log::TYPE_MAIN
#include "Log.h"
#define ENABLE_SCOPE_INOUT
#include "ScopeInOut.h"

using namespace test;
using namespace Utils;

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

void set_timefunc(void)
{
	auto Func = [](char *buf, int size) {
		struct timespec ts;
		struct tm lt;

		clock_gettime(CLOCK_REALTIME, &ts);
		localtime_r(&ts.tv_sec, &lt);

		char tmp[32];
		strftime(tmp, 32, "%Y/%m/%d %H:%M:%S", &lt);

		int msec = ts.tv_nsec / 1000;
		snprintf(buf, size, "%s.%06d", tmp, msec);
	};
	Log::SetTimeFunc(Func);
}

/////////////////////////////////////////////
//
// Log Test
//
/////////////////////////////////////////////
#define LOG_NONE_PINK(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_PINK,  Utils::Log::LEVEL_NONE, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
bool test_2_1_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	TEST_LOG("Log Test");

	// close 
	VERIFY(Log::CloseFile() == true);
	Log::SetKeepOpen(false);
	Log::SetFileName("utils2.log");

	Log::SetToStdOut(true);
	VERIFY(Log::GetToStdOut() == true);
	Log::SetToFile(true);
	VERIFY(Log::GetToFile() == true);

	set_timefunc();

	Log::SetLevel(Log::TYPE_MAIN, Log::LEVEL_DBG);
	LOG_ERR("output to stdout");
	LOG_WARN("output to stdout");
	LOG_DBG("output to stdout");
	LOG_INFO("output to stdout");

	LOG_INFO_GREEN("INFO GREEN");
	LOG_INFO_CYAN("INFO CYAN");
	LOG_INFO_PINK("INFO PINK");

	LOG_DBG_GREEN("DBG GREEN");
	LOG_DBG_CYAN("DBG CYAN");
	LOG_DBG_PINK("DBG PINK");

	Log::SetLevel(Log::TYPE_MAIN, Log::LEVEL_MAX); // invalid level
	Log::SetLevel(Log::TYPE_MAIN, Log::LEVEL_WARN);
	LOG_DBG_PINK("can not output"); // higher than WARN
	LOG_NONE_PINK("NONE PINK (invalid usage)");

	// recovery
	Log::SetKeepOpen(true);
	Log::SetToStdOut(false);
	Log::SetToFile(true);
	VERIFY(Log::OpenFile("utils2.log", true) == true);
	return true;
}

#define LOG_T212(A, ...) Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_NONE, Utils::Log::LEVEL_DBG, LogID), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
bool test_2_1_2(void *This)
{
	TestSample *Test = (TestSample *)This;
	uint16_t LogID = 0x1234;
	const char *LogStr = "T212";

	LOG_T212("%s", "LevelNone");
	VERIFY(Log::SetString(LogID, NULL) == false);
	VERIFY(Log::SetLevel(LogID, Log::LEVEL_DBG) == true);

	LOG_T212("%s", "String Not Set");
	VERIFY(Log::SetString(LogID, LogStr) == true);

	LOG_T212("Test LogID(%s) will be output", LogStr);

	VERIFY(Log::UnSet(LogID) == true); // Unset

	VERIFY(Log::SetLevel(LogID, Log::LEVEL_DBG) == true);
	VERIFY(Log::UnSet(LogID) == true); // fail StrMap

	VERIFY(Log::SetString(LogID, "T212") == true);
	VERIFY(Log::UnSet(LogID) == true); // fail LevelMap

	return true;
}
/////////////////////////////////////////////
//
// Config(Dump) Test
//
/////////////////////////////////////////////
bool test_2_2_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(Dump)");

	cif1 = Config::Create("DUMP"); // new Config
	VERIFY(cif1 != NULL);

	VERIFY(cif1->Define("String1", "yes", "yes|no", 3) == true);
	VERIFY(cif1->Define("Integer1", 100, 50, 500) == true);
	VERIFY(cif1->Define("Float1", 10.0f, 5.0f, 50.0f) == true);
	cif1->Dump();

	VERIFY(Config::Destroy("DUMP") == true);
	return true;
}

} // namespace

void TestSample::InitializeOnce(void)
{
	_TEST_LOG("START Once");

	Log::SetLevel(Log::TYPE_UTILS, Log::LEVEL_DBG);
	Log::SetLevel(Log::TYPE_MAIN, Log::LEVEL_WARN);
	Log::OpenFile("utils2.log");
	Log::SetToStdOut(false);

	LOG_WARN("START Once\n");
}

void TestSample::FinalizeOnce(void)
{
	_TEST_LOG("STOP Once");
	LOG_WARN("STOP Once\n");

	Log::CloseFile();
}

void TestSample::InitializePerTest(void)
{
	_TEST_LOG("TEST START %s", GetTestID());
	LOG_WARN("TEST START %s", GetTestID());
}

void TestSample::FinalizePerTest(void)
{
	_TEST_LOG("TEST STOP %s", GetTestID());
	LOG_WARN("TEST STOP %s\n", GetTestID());
}

bool TestSample::RegisterTests(void)
{
	Register("u2.1.1", test_2_1_1);
	Register("u2.1.2", test_2_1_2);
	Register("u2.2.1", test_2_2_1);
	return true;
}

const char *TestSample::OwnName(void)
{
	return "LogTest";
}

