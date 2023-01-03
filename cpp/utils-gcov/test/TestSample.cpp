#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>

#include "TestBase.h"
#include "Config.h"
#include "Thread.h"
#include "Timer.h"
#include "Event.h"
#include "Observer.h"
#include "String.h"
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

#define LOOP_NUM 50000
int Buffer[LOOP_NUM*3] = {};
int Counter = 0;

typedef struct {
	Thread::ThreadIF *tif;
	Event::EventIF<int> event;
} THREAD_PARAM;

void ThreadTest1(void *This, THREAD_PARAM *tp, int index)
{
	struct timespec req;
	struct timespec rem;
	int index_save = index;
	int loop = 0;
	int fail = 0;

	auto TestFunc = [&](void) {
		Buffer[Counter] = Counter;
		Counter++;
		if(Counter > LOOP_NUM*3) {
			TEST_LOG("Counter overflow %d", Counter);
			exit(1);
		}
		tp->event.WakeMsg(loop);

		loop++;
		if(loop % 5000 == 0)
			TEST_LOG("[%d]=%d", index_save, loop);
	};

	TEST_LOG("START [%d]", index_save);
	for(int i = 0; i < LOOP_NUM; i++) {
		tp->tif->Enqueue(TestFunc);

		req.tv_sec = 0;
		req.tv_nsec = 50;
		nanosleep(&req, NULL);

		int ret = tp->event.WaitMsg();
		if(ret != i)
			fail++;
	}
	TEST_LOG("END [%d], fail=%d", index_save, fail);
}

bool ThreadVerify(void *This)
{
	int fail = 0;
	bool ret = true;
	TestSample *Test = (TestSample *)This;

	for(int i = 0; i < LOOP_NUM*3; i++) {
		if(Buffer[i] != i)
			fail++;
		//printf("Buf[%d]=%d\n", i, Buffer[i]);
	}
	TEST_LOG("<<<< ThreadVerify: fail=%d, Counter=%d", fail, Counter);

	if(fail != 0)
		ret = false;
	return ret;
}

/////////////////////////////////////////////
//
// Observer Test
//
/////////////////////////////////////////////
bool test_1_1_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	int received = 0;
	bool ret = false;
	Observer::Msg_t m = { "Hoge" };
	Observer::ObserverIF *oif1 = NULL;

	TEST_LOG("Observer Test");

	oif1 = Observer::Create("Inst");
	VERIFY(oif1 != NULL);
	VERIFY(Observer::Create("Inst") == NULL); // same name
	VERIFY(Observer::Create(NULL) == NULL); // invalid name

	ret = oif1->Subscribe("recv1",[&](Observer::Msg_t Msg){ if(Msg.Str == "Hoge") received++; } );
	VERIFY(ret == true);
	ret = oif1->Subscribe("recv2",[&](Observer::Msg_t Msg){ if(Msg.Str == "Hoge") received++; } );
	VERIFY(ret == true);
	ret = oif1->Subscribe("recv3",[&](Observer::Msg_t Msg){ if(Msg.Str == "Hoge") received++; } );
	VERIFY(ret == true);

	oif1 = Observer::GetInstance("ZZZ");
	VERIFY(oif1 == NULL);
	oif1 = Observer::GetInstance("Inst");
	VERIFY(oif1 != NULL);

	VERIFY(oif1->Notify(m) == true);
	TEST_LOG("received=%d", received);
	VERIFY(received == 3);

	m.Str = "XXX"; // invalid string
	VERIFY(oif1->Notify(m) == true);
	TEST_LOG("received=%d", received);
	VERIFY(received == 3);

	VERIFY(oif1->UnSubscribe("recv1") == true);
	VERIFY(oif1->UnSubscribe("recv2") == true);
	VERIFY(oif1->UnSubscribe("recv3") == true);
	VERIFY(oif1->UnSubscribe("YYY") == false); // not subscribed

	VERIFY(Observer::Destroy("Inst") == true);
	VERIFY(Observer::Destroy("ZZZ") == false); // not created

	return true;
}

/////////////////////////////////////////////
//
// Thread Test
//
/////////////////////////////////////////////
bool test_1_2_1(void *This)
{
	SCOPE_INOUT_DELTA(Log::LEVEL_WARN);
	TestSample *Test = (TestSample *)This;
	Thread::ThreadIF *tif1 = NULL;

	TEST_LOG("Thread Test1");
	Counter = 0;
	memset(Buffer, 0, sizeof(Buffer));

	tif1 = Thread::GetInstance(Thread::MODEL);
	VERIFY(tif1 != NULL);
	THREAD_PARAM tp1 = {.tif = tif1};
	THREAD_PARAM tp2 = {.tif = tif1};
	THREAD_PARAM tp3 = {.tif = tif1};

	TEST_LOG("LOOP=%d", LOOP_NUM);
	std::thread thr1(ThreadTest1, This, &tp1, 0);
	std::thread thr2(ThreadTest1, This, &tp2, 1);
	std::thread thr3(ThreadTest1, This, &tp3, 2);

	thr1.join();
	thr2.join();
	thr3.join();
	TEST_LOG("JOIN OK");

	VERIFY(ThreadVerify(This) == true);

	return true;
}

bool test_1_2_2(void *This)
{
	SCOPE_INOUT(Log::LEVEL_WARN);
	TestSample *Test = (TestSample *)This;
	Thread::ThreadIF *tif1 = NULL;
	int hoge = 0;
	uint64_t ID = 0;

	TEST_LOG("Thread Test2");

	VERIFY(Thread::GetInstance("ZZZ") == NULL); // not found

	VERIFY(Thread::Create(NULL) == NULL); // invalid name
	tif1 = Thread::Create("YYY"); // new thread
	VERIFY(tif1 != NULL);

	auto TestFunc = [&](void) {
		hoge = 123;
		ID = Thread::GetID();
	};
	VERIFY(tif1->Enqueue(TestFunc) == true);
	VERIFY(tif1->Enqueue(NULL) == false); // invalid func
	usleep(1000*100);

	VERIFY(hoge == 123);
	VERIFY(ID != 0);

	VERIFY(Thread::Destroy("YYY") == true); // delete thread
	VERIFY(Thread::Destroy("YYY") == false); // already delete

	return true;
}

/////////////////////////////////////////////
//
// Timer Test
//
/////////////////////////////////////////////
bool test_1_3_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	Timer::TimerIF *tif1 = NULL;

	TEST_LOG("Timer Test");

	VERIFY(Timer::GetInstance("ZZZ") == NULL); // not found

	VERIFY(Timer::Create(NULL) == NULL); // invalid name
	tif1 = Timer::Create("YYY"); // new timer
	VERIFY(tif1 != NULL);

	VERIFY(tif1->SetTick(1000 * 200) == true);
	auto func = []() {

	};
	VERIFY(tif1->Add("1secTimer", func, 1000) == true);
	VERIFY(tif1->Add("10msecTimer", func, 10) == false); // too low interval
	VERIFY(tif1->Add("2secTimer", NULL, 2000) == false); // null po

	VERIFY(tif1->SetTick(0) == false); // zero tick
	VERIFY(tif1->SetTick(1000 * 2000) == false); // too high tick

	//// START ////
	VERIFY(tif1->Stop() == false); // not run
	VERIFY(tif1->Start() == true);
	VERIFY(tif1->SetTick(1000*300) == false); // already started

	//// STOP ////
	VERIFY(tif1->Start() == false); // already started
	VERIFY(tif1->Stop() == true);

	VERIFY(tif1->Remove("not found") == false); // not found
	VERIFY(tif1->Remove("1secTimer") == true);
	VERIFY(tif1->Remove("1secTimer") == false); // already delete

	VERIFY(Timer::Destroy("YYY") == true);
	VERIFY(Timer::Destroy("YYY") == false); // already delete
	return true;
}

bool test_1_3_2(void *This)
{
	TestSample *Test = (TestSample *)This;
	Timer::TimerIF *tif1 = NULL;
	TEST_LOG("Timer Test2");

	tif1 = Timer::Create("TTT"); // new timer
	VERIFY(tif1 != NULL);

	for(int i = 0; i < 2; i++) {
		int num1 = 0, num2 = 0, num3 = 0;

		VERIFY(tif1->SetTick(1000 * 100) == true);
		auto func1 = [&]() { num1++; };
		auto func2 = [&]() { num2++; };
		auto func3 = [&]() { num3++; };
		VERIFY(tif1->Add("100msecTimer", func1, 100, 17) == true);
		VERIFY(tif1->Add("300msecTimer", func2, 300, 5) == true);
		VERIFY(tif1->Add("1000msecTimer", func3, 1000) == true);
		VERIFY(tif1->Start() == true);
    
		sleep(2);
		usleep(1000*150);
		VERIFY(tif1->Remove("1000msecTimer") == true);
		VERIFY(tif1->Stop() == true);
    
		TEST_LOG("[%d] num1=%d, num2=%d, num3=%d", i, num1, num2, num3);
		VERIFY(num1 == 17);
		VERIFY(num2 == 5);
		VERIFY(num3 == 2);
	}

	VERIFY(Timer::Destroy("TTT") == true);
	return true;
}

/////////////////////////////////////////////
//
// Config Test
//
/////////////////////////////////////////////
bool test_1_4_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(INT)");

	cif1 = Config::Create("INTEGER"); // new Config
	VERIFY(cif1 != NULL);
	VERIFY(cif1 == Config::GetInstance("INTEGER"));
	VERIFY(Config::Create(NULL) == NULL); // null po

	// define
	VERIFY(cif1->Define("Value1", 100, 50, 500) == true);
	VERIFY(cif1->Define("Value2", 10, -50, 50) == true);
	// get init value
	VERIFY(cif1->GetInteger("Value1") == 100);
	VERIFY(cif1->GetInteger("Value2") == 10);

	// min value
	VERIFY(cif1->Set("Value1", 50) == true);
	VERIFY(cif1->Set("Value2", -50) == true);
	VERIFY(cif1->GetInteger("Value1") == 50);
	VERIFY(cif1->GetInteger("Value2") == -50);

	// max value
	VERIFY(cif1->Set("Value1", 500) == true);
	VERIFY(cif1->Set("Value2", 50) == true);
	VERIFY(cif1->GetInteger("Value1") == 500);
	VERIFY(cif1->GetInteger("Value2") == 50);

	// reset Value1
	VERIFY(cif1->Reset("not found") == false); // not found
	VERIFY(cif1->Reset("Value1") == true);
	VERIFY(cif1->GetInteger("Value1") == 100); // init value
	// reset all
	VERIFY(cif1->Set("Value1", 66) == true);
	VERIFY(cif1->Reset() == true); // all reset
	VERIFY(cif1->GetInteger("Value1") == 100); // init value
	VERIFY(cif1->GetInteger("Value2") == 10); // init value

	// invalid param
	VERIFY(cif1->Set("Value1", 49) == false);
	VERIFY(cif1->Set("Value1", 501) == false);
	VERIFY(cif1->Set(NULL, 100) == false);
	VERIFY(cif1->GetInteger(NULL) == false);
	VERIFY(cif1->Define(NULL, 100, 50, 500) == false);
	VERIFY(cif1->Define("Hoge", 100, 1000, 500) == false);
	VERIFY(cif1->Define("Hoge", -100, 500, 1000) == false);
	VERIFY(cif1->Define("Hoge", 10000, 500, 1000) == false);

	VERIFY(Config::Destroy("INTEGER") == true);
	return true;
}

bool test_1_4_2(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(Float)");

	cif1 = Config::Create("FLOAT"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("Value1", 100.0f, 50.0f, 500.0f) == true);
	VERIFY(cif1->Define("Value2", 10.0f, -50.0f, 50.0f) == true);
	// get init value
	VERIFY(cif1->GetFloat("Value1") == 100.0f);
	VERIFY(cif1->GetFloat("Value2") == 10.0f);

	// min value
	VERIFY(cif1->Set("Value1", 50.0f) == true);
	VERIFY(cif1->Set("Value2", -50.0f) == true);
	VERIFY(cif1->GetFloat("Value1") == 50.0f);
	VERIFY(cif1->GetFloat("Value2") == -50.0f);

	// max value
	VERIFY(cif1->Set("Value1", 500.0f) == true);
	VERIFY(cif1->Set("Value2", 50.0f) == true);
	VERIFY(cif1->GetFloat("Value1") == 500.0f);
	VERIFY(cif1->GetFloat("Value2") == 50.0f);

	// reset Value1
	VERIFY(cif1->Reset("not found") == false); // not found
	VERIFY(cif1->Reset("Value1") == true);
	VERIFY(cif1->GetFloat("Value1") == 100.0f); // init value
	// reset all
	VERIFY(cif1->Set("Value1", 66.6f) == true);
	VERIFY(cif1->Reset() == true); // all reset
	VERIFY(cif1->GetFloat("Value1") == 100.0f); // init value
	VERIFY(cif1->GetFloat("Value2") == 10.0f); // init value

	// invalid param
	VERIFY(cif1->Set("Value1", 49.0f) == false);
	VERIFY(cif1->Set("Value1", 501.0f) == false);
	VERIFY(cif1->Set(NULL, 100.0f) == false);
	VERIFY(cif1->GetFloat(NULL) == false);
	VERIFY(cif1->Define(NULL, 100.0f, 50.0f, 500.0f) == false);
	VERIFY(cif1->Define("Hoge", 100.0f, 1000.0f, 500.0f) == false);
	VERIFY(cif1->Define("Hoge", -100.0f, 500.0f, 1000.0f) == false);
	VERIFY(cif1->Define("Hoge", 10000.0f, 500.0f, 1000.0f) == false);

	VERIFY(Config::Destroy("FLOAT") == true);
	return true;
}

bool test_1_4_3(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(String)");

	cif1 = Config::Create("STRING"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("String1", "abc", "[a-z]*", 16) == true);
	VERIFY(cif1->Define("String2", "123", "[0-9]*", 8) == true);
	// get init value
	VERIFY_STR(cif1->GetString("String1"), "abc");
	VERIFY_STR(cif1->GetString("String2"), "123");

	// max value
	VERIFY(cif1->Set("String1", "abcdefghijklmnop") == true);
	VERIFY(cif1->Set("String2", "12345678") == true);
	VERIFY_STR(cif1->GetString("String1"), "abcdefghijklmnop");
	VERIFY_STR(cif1->GetString("String2"), "12345678");

	// reset Value1
	VERIFY(cif1->Reset("not found") == false); // not found
	VERIFY(cif1->Reset("String1") == true);
	VERIFY_STR(cif1->GetString("String1"), "abc"); // init value
	// reset all
	VERIFY(cif1->Set("String1", "hogehoge") == true);
	VERIFY(cif1->Reset() == true); // all reset
	VERIFY_STR(cif1->GetString("String1"), "abc"); // init value
	VERIFY_STR(cif1->GetString("String2"), "123"); // init value

	// invalid param
	VERIFY(cif1->Set("String1", "abcdefghijklmnopq") == false);
	VERIFY(cif1->Set("String1", "12345") == false);
	VERIFY(cif1->Set("String1", "ABCDE") == false);
	VERIFY(cif1->Set("String2", "abcde") == false);
	VERIFY(cif1->Set(NULL, "Hoge") == false);
	VERIFY_STR(cif1->GetString(NULL), "");
	VERIFY(cif1->Define(NULL, "abc", "[a-z]*", 16) == false);
	VERIFY(cif1->Define("String3", NULL, "[a-z]*", 16) == false);
	VERIFY(cif1->Define("String3", "abc", NULL, 16) == false);
	VERIFY(cif1->Define("String3", "abc", "[a-z]*", 2) == false);
	VERIFY(cif1->Define("String3", "ABC", "[a-z]*", 16) == false);

	VERIFY(Config::Destroy("STRING") == true);
	return true;
}

bool test_1_4_4(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(Load/Save)");

	cif1 = Config::Create("SIF"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("String1", "yes", "yes|no", 3) == true);
	VERIFY(cif1->Define("String2", "1234abcd",  "[0-9a-f]*", 16) == true);
	VERIFY(cif1->Define("Integer", 100,  50, 500) == true);
	VERIFY(cif1->Define("Float", 10.0f,  5.0f, 50.0f) == true);

	// Set new value
	VERIFY(cif1->Set("String1", "no") == true);
	VERIFY(cif1->Set("String2", "abcdef0123456789") == true);
	VERIFY(cif1->Set("Integer", 123) == true);
	VERIFY(cif1->Set("Float", 34.5f) == true);

	// Save
	VERIFY(cif1->Save(NULL) == false); // null po
	VERIFY(cif1->Save("./utils.conf") == true);

	// all reset before Load
	VERIFY(cif1->Reset() == true);
	VERIFY_STR(cif1->GetString("String1"), "yes"); // cmp to init value
	VERIFY_STR(cif1->GetString("String2"), "1234abcd");
	VERIFY(cif1->GetInteger("Integer") == 100);
	VERIFY(cif1->GetFloat("Float") == 10.0f);

	// Load
	VERIFY(cif1->Load(NULL) == false); // null po
	VERIFY(cif1->Load("./utils.conf") == true);
	VERIFY_STR(cif1->GetString("String1"), "no");
	VERIFY_STR(cif1->GetString("String2"), "abcdef0123456789");
	VERIFY(cif1->GetInteger("Integer") == 123);
	VERIFY(cif1->GetFloat("Float") == 34.5f);

	VERIFY(Config::Destroy("SIF") == true);
	return true;
}

bool test_1_4_5(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(regex)");

	cif1 = Config::Create("REGEX"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("String1", "yes", "yes|no", 3) == true);

	// OK
	VERIFY(cif1->Set("String1", "no") == true);    TEST_LOG("String1=%s", cif1->GetString("String1"));
	VERIFY(cif1->Set("String1", "yes") == true);   TEST_LOG("String1=%s", cif1->GetString("String1"));

	// NG
	VERIFY(cif1->Set("String1", "nos") == false);  TEST_LOG("String1=%s", cif1->GetString("String1"));
	VERIFY(cif1->Set("String1", "y") == false);    TEST_LOG("String1=%s", cif1->GetString("String1"));
	VERIFY(cif1->Set("String1", "yess") == false); TEST_LOG("String1=%s", cif1->GetString("String1"));
	VERIFY(cif1->Set("String1", "no ") == false);  TEST_LOG("String1=%s", cif1->GetString("String1"));
	VERIFY(cif1->Set("String1", " no") == false);  TEST_LOG("String1=%s", cif1->GetString("String1"));
	VERIFY(cif1->Set("String1", "xyz") == false);  TEST_LOG("String1=%s", cif1->GetString("String1"));

	VERIFY(Config::Destroy("REGEX") == true);
	return true;
}

bool test_1_4_6(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	TEST_LOG("Config Test(Load/Save)");

	cif1 = Config::Create("SYNTAX"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("String1", "yes", "yes|no", 3) == true);
	VERIFY(cif1->Define("Integer", 100,  50, 500) == true);

	// create new file 
	FILE *fp = fopen("./.tmp.conf", "w");
	fprintf(fp, " String1   =  no  # Comment\n");
	fprintf(fp, "Integer=77#Comment\n");
	fprintf(fp, "# Integer=1234\n"); // ignore
	fclose(fp);

	// Load
	VERIFY(cif1->Load("./.tmp.conf") == true);
	VERIFY_STR(cif1->GetString("String1"), "no");
	VERIFY(cif1->GetInteger("Integer") == 77);

	VERIFY(Config::Destroy("SYNTAX") == true);
	return true;
}

/////////////////////////////////////////////
//
// String Test
//
/////////////////////////////////////////////
bool test_1_5_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	std::string str = "abc";
	std::string str2 = "def";
	std::smatch sm;

	VERIFY(String::MatchAll(str, sm, NULL) == false);
	VERIFY(String::MatchAll(str, sm, "abc|def") == true);
	VERIFY(String::MatchAll(str2, sm, "(abc|def)") == true);
	VERIFY(sm[1] == "def");

	String::Replace(str, NULL, "A");
	String::Replace(str, "B", NULL);

	str = " abc defa bc ";
	String::Replace(str, " ", "");
	String::Replace(str, "abc", "ABC");
	VERIFY(str == "ABCdefABC");

	return true;
}



} // namespace

void TestSample::InitializeOnce(void)
{
	_TEST_LOG("START Once");

	Log::SetLevel(Log::TYPE_UTILS, Log::LEVEL_DBG);
	Log::SetLevel(Log::TYPE_MAIN, Log::LEVEL_WARN);
	Log::OpenFile("utils.log");
	Log::SetToStdOut(false);
	Thread::Initialize();

	LOG_WARN("START Once\n");
}

void TestSample::FinalizeOnce(void)
{
	_TEST_LOG("STOP Once");
	LOG_WARN("STOP Once\n");

	Thread::Finalize();
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
	Register("u1.1.1", test_1_1_1);
	Register("u1.2.1", test_1_2_1);
	Register("u1.2.2", test_1_2_2);
	Register("u1.3.1", test_1_3_1);
	Register("u1.3.2", test_1_3_2);

	Register("u1.4.1", test_1_4_1);
	Register("u1.4.2", test_1_4_2);
	Register("u1.4.3", test_1_4_3);
	Register("u1.4.4", test_1_4_4);
	Register("u1.4.5", test_1_4_5);
	Register("u1.4.6", test_1_4_6);

	Register("u1.5.1", test_1_5_1);

	return true;
}

const char *TestSample::OwnName(void)
{
	return "UtilsTest";
}

