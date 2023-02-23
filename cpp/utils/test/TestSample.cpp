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
#include "Library.h"
#define MY_LOG_TYPE Log::TYPE_MAIN
#include "Log.h"
#define ENABLE_SCOPE_INOUT
#include "ScopeInOut.h"
#include "FactoryTest.h"

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
	Observer::ObserverIF *oif2 = NULL;

	TEST_LOG("Observer Test");

	oif1 = Observer::Create("Inst");
	VERIFY(oif1 != NULL);
	VERIFY(Observer::Create("Inst") == oif1); // same name
	oif2 = Observer::Create(NULL); // get null object
	VERIFY(oif2 != NULL);
	VERIFY(oif2->Subscribe(NULL,NULL) == true);
	VERIFY(oif2->UnSubscribe(NULL) == true);
	VERIFY(oif2->Notify(m) == true);

	ret = oif1->Subscribe("recv1",[&](Observer::Msg_t Msg){ if(Msg.Str == "Hoge") received++; } );
	VERIFY(ret == true);
	ret = oif1->Subscribe("recv2",[&](Observer::Msg_t Msg){ if(Msg.Str == "Hoge") received++; } );
	VERIFY(ret == true);
	ret = oif1->Subscribe("recv3",[&](Observer::Msg_t Msg){ if(Msg.Str == "Hoge") received++; } );
	VERIFY(ret == true);

	oif1 = Observer::GetInstance("ZZZ");
	VERIFY(oif1 == oif2);
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
	Thread::ThreadIF *tif2 = NULL;
	int hoge = 0;
	uint64_t ID = 0;

	TEST_LOG("Thread Test2");

	tif2 = Thread::GetInstance("ZZZ");// get null object
	VERIFY(tif2 != NULL);
	VERIFY(tif2->Enqueue(NULL) == true);

	VERIFY(Thread::Create(NULL) == tif2); // invalid name
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
	Timer::TimerIF *tif2 = NULL;

	TEST_LOG("Timer Test");

	tif2 = Timer::GetInstance("ZZZ"); // get null object
	VERIFY(tif2 != NULL);
	VERIFY(tif2->SetTick(0) == true);
	VERIFY(tif2->Add(NULL, NULL, 0) == true);
	VERIFY(tif2->Remove(0) == true);
	VERIFY(tif2->Start() == true);
	VERIFY(tif2->Stop() == true);

	VERIFY(Timer::Create(NULL) == tif2); // invalid name
	tif1 = Timer::Create("YYY"); // new timer
	VERIFY(tif1 != NULL);
	VERIFY(Timer::GetInstance("YYY") == tif1);

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
bool VerifyNullObj(Config::ConfigIF *cif2, void *This)
{
	VERIFY(cif2->Define(NULL, 0, 0, 0) == true);
	VERIFY(cif2->Define(NULL,0.0f, 0.0f, 0.0f) == true);
	VERIFY(cif2->Define(NULL, "a", "b", 1) == true);

	VERIFY(cif2->Set(NULL, 0) == true);
	VERIFY(cif2->Set(NULL, 0.0f) == true);
	VERIFY(cif2->Set(NULL, "a") == true);
	VERIFY_STR(cif2->GetString(NULL), "");
	VERIFY(cif2->GetInteger(NULL) == 0);
	VERIFY(cif2->GetFloat(NULL) == 0.0f);

	VERIFY(cif2->Reset(NULL) == true);
	VERIFY(cif2->Subscribe(NULL, NULL, NULL) == true);
	VERIFY(cif2->UnSubscribe(NULL, NULL) == true);

	VERIFY(cif2->Load(NULL) == true);
	VERIFY(cif2->Save(NULL) == true);
	cif2->Dump();

	return true;
}

bool test_1_4_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	Config::ConfigIF *cif2 = NULL;
	TEST_LOG("Config Test(INT)");

	cif1 = Config::Create("INTEGER"); // new Config
	VERIFY(cif1 != NULL);
	VERIFY(cif1 == Config::GetInstance("INTEGER"));
	VERIFY((cif2 = Config::GetInstance(NULL)) != NULL); // get null obj
	VERIFY(Config::Create(NULL) == cif2);

	VERIFY(VerifyNullObj(cif2, This) == true);

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
	fprintf(fp, "NotDefined = NULL\n"); // ignore
	fclose(fp);

	// Load
	VERIFY(cif1->Load("./.tmp.conf") == true);
	VERIFY_STR(cif1->GetString("String1"), "no");
	VERIFY(cif1->GetInteger("Integer") == 77);

	VERIFY(Config::Destroy("SYNTAX") == true);
	return true;
}

typedef enum {
	ENUM_TEST_A,
	ENUM_TEST_B,
	ENUM_TEST_C,
	ENUM_TEST_MAX,
} ENUM_TEST;

bool test_1_4_7(void *This)
{
	TestSample *Test = (TestSample *)This;
	Config::ConfigIF *cif1 = NULL;
	ENUM_TEST ev;
	TEST_LOG("Config Test(Enum)");

	cif1 = Config::Create("ENUM"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("Value1", ENUM_TEST_B, ENUM_TEST_A, ENUM_TEST_C) == true);
	// get init value
	VERIFY(cif1->GetInteger("Value1") == ENUM_TEST_B);

	// set min value(error)
	VERIFY(cif1->Set("Value1", -1) == false);
	// set max value(error)
	VERIFY(cif1->Set("Value1", ENUM_TEST_MAX) == false);

	// set min value
	VERIFY(cif1->Set("Value1", ENUM_TEST_A) == true);
	ev = (ENUM_TEST)cif1->GetInteger("Value1");
	VERIFY(ev == ENUM_TEST_A);

	// set max value
	VERIFY(cif1->Set("Value1", ENUM_TEST_C) == true);
	ev = (ENUM_TEST)cif1->GetInteger("Value1");
	VERIFY(ev == ENUM_TEST_C);

	VERIFY(Config::Destroy("ENUM") == true);
	return true;
}

bool test_1_4_8(void *This)
{
	Config::ConfigIF *cif1 = NULL;
	int32_t iv1 = 0;
	int32_t iv2 = 0;
	TEST_LOG("Config Test(Int-Subscribe)");

	cif1 = Config::Create("Int-Subscribe"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("Value1", 20, 10, 30) == true);

	// subscribe
	auto func1 = [&](Config::ConfigIF *CIF) {
		iv1 = CIF->GetInteger("Value1");
	};
	auto func2 = [&](Config::ConfigIF *CIF) {
		iv2 = CIF->GetInteger("Value1");
	};
	VERIFY(cif1->Subscribe("Value1", "Reader1", func1) == true);
	VERIFY(cif1->Subscribe("Value1", "Reader2", func2) == true);

	VERIFY(cif1->Subscribe("Value1", NULL, func1) == false);
	VERIFY(cif1->Subscribe("Value1", "Reader3", NULL) == false);
	VERIFY(cif1->Subscribe(NULL, "Reader1", func1) == false);

	// set new value (with subscribe +2)
	VERIFY(cif1->Set("Value1", 15) == true);
	VERIFY_EQ(iv1, 15); // modfified
	VERIFY_EQ(iv2, 15); // modfified

	iv1 = 1000;
	iv2 = 2000;

	// set same value (with subscribe +2)
	VERIFY(cif1->Set("Value1", 15) == true);
	VERIFY_EQ(iv1, 1000); // keep
	VERIFY_EQ(iv2, 2000); // keep

	VERIFY(cif1->UnSubscribe("Value1", "Reader1") == true);

	// set new value (with subscribe +1)
	VERIFY(cif1->Set("Value1", 25) == true);
	VERIFY_EQ(iv1, 1000); // keep
	VERIFY_EQ(iv2, 25);   // modfified

	VERIFY(cif1->UnSubscribe("Value1", "NotFound") == false);
	VERIFY(cif1->UnSubscribe("Value1", NULL) == false);
	VERIFY(cif1->UnSubscribe(NULL, "Reader2") == false);
	VERIFY(cif1->UnSubscribe("Value1", "Reader2") == true);

	// set new value(no subscribe)
	VERIFY(cif1->Set("Value1", 30) == true);
	VERIFY_EQ(iv1, 1000); // keep
	VERIFY_EQ(iv2, 25);   // keep
	VERIFY_EQ(cif1->GetInteger("Value1"), 30);

	VERIFY(Config::Destroy("Int-Subscribe") == true);
	return true;
}

bool test_1_4_9(void *This)
{
	Config::ConfigIF *cif1 = NULL;
	float fv1 = 0;
	float fv2 = 0;
	TEST_LOG("Config Test(Float-Subscribe)");

	cif1 = Config::Create("Float-Subscribe"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("Value1", 20.0f, 10.0f, 30.0f) == true);

	// subscribe
	auto func1 = [&](Config::ConfigIF *CIF) {
		fv1 = CIF->GetFloat("Value1");
	};
	auto func2 = [&](Config::ConfigIF *CIF) {
		fv2 = CIF->GetFloat("Value1");
	};
	VERIFY(cif1->Subscribe("Value1", "Reader1", func1) == true);
	VERIFY(cif1->Subscribe("Value1", "Reader2", func2) == true);

	// set new value (with subscribe +2)
	VERIFY(cif1->Set("Value1", 15.1f) == true);
	VERIFY_EQ(fv1, 15.1f); // modfified
	VERIFY_EQ(fv2, 15.1f); // modfified

	fv1 = 1000.0f;
	fv2 = 2000.0f;

	// set same value (with subscribe +2)
	VERIFY(cif1->Set("Value1", 15.1f) == true);
	VERIFY_EQ(fv1, 1000.0f); // keep
	VERIFY_EQ(fv2, 2000.0f); // keep

	VERIFY(cif1->UnSubscribe("Value1", "Reader1") == true);

	// set new value (with subscribe +1)
	VERIFY(cif1->Set("Value1", 25.2f) == true);
	VERIFY_EQ(fv1, 1000.0f); // keep
	VERIFY_EQ(fv2, 25.2f);   // modfified

	VERIFY(cif1->UnSubscribe("Value1", "Reader2") == true);

	// set new value(no subscribe)
	VERIFY(cif1->Set("Value1", 27.3f) == true);
	VERIFY_EQ(fv1, 1000.0f); // keep
	VERIFY_EQ(fv2, 25.2f);   // keep
	VERIFY_EQ(cif1->GetFloat("Value1"), 27.3f);

	VERIFY(Config::Destroy("Float-Subscribe") == true);
	return true;
}

bool test_1_4_10(void *This)
{
	Config::ConfigIF *cif1 = NULL;
	std::string sv1 = "hoge";
	std::string sv2 = "hoge";
	std::string tmp;
	TEST_LOG("Config Test(String-Subscribe)");

	cif1 = Config::Create("String-Subscribe"); // new Config
	VERIFY(cif1 != NULL);

	// define
	VERIFY(cif1->Define("String1", "yes", "yes|no", 3) == true);
	VERIFY(cif1->Define("String2", "On", "On|Off", 3) == true);

	// subscribe
	auto func1 = [&](Config::ConfigIF *CIF) {
		sv1 = CIF->GetString("String1");
	};
	auto func2 = [&](Config::ConfigIF *CIF) {
		sv2 = CIF->GetString("String2");
	};
	VERIFY(cif1->Subscribe("String1", "Reader1", func1) == true);
	VERIFY(cif1->Subscribe("String2", "Reader2", func2) == true);

	// set new value (with subscribe +2)
	VERIFY(cif1->Set("String1", "no") == true);
	VERIFY_EQ(sv1, "no");   // modfified
	VERIFY_EQ(sv2, "hoge"); // keep

	// set new value (with subscribe +2)
	VERIFY(cif1->Set("String2", "Off") == true);
	VERIFY_EQ(sv1, "no");  // keep
	VERIFY_EQ(sv2, "Off"); // modified

	sv1 = "123";
	sv2 = "456";

	// set same value (with subscribe +2)
	VERIFY(cif1->Set("String1", "no") == true);
	VERIFY(cif1->Set("String2", "Off") == true);
	VERIFY_EQ(sv1, "123"); // keep
	VERIFY_EQ(sv2, "456"); // keep

	VERIFY(cif1->UnSubscribe("String1", "Reader1") == true);

	// set new value (with subscribe +1)
	VERIFY(cif1->Set("String1", "yes") == true);
	VERIFY(cif1->Set("String2", "On") == true);
	VERIFY_EQ(sv1, "123"); // keep
	VERIFY_EQ(sv2, "On");  // modified
	tmp = cif1->GetString("String1"); VERIFY_EQ(tmp, "yes");

	VERIFY(cif1->UnSubscribe("String2", "Reader2") == true);

	// set new value(no subscribe)
	VERIFY(cif1->Set("String1", "no") == true);
	VERIFY(cif1->Set("String2", "Off") == true);
	VERIFY_EQ(sv1, "123"); // keep
	VERIFY_EQ(sv2, "On");  // keep
	tmp = cif1->GetString("String1"); VERIFY_EQ(tmp, "no");
	tmp = cif1->GetString("String2"); VERIFY_EQ(tmp, "Off");

	VERIFY(Config::Destroy("String-Subscribe") == true);
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

/////////////////////////////////////////////
//
// Library Test
//
/////////////////////////////////////////////
bool test_1_6_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	std::string dir1 = getenv("UTILS_TEST_DIR");
	std::string dir2;
	dir2 = dir1 + "/test/.objs";

	VERIFY(Library::Load(NULL) == false);
	VERIFY(Library::Load("./not_exists") == false);
	VERIFY(Library::Load(dir2.c_str()) == false);
	Library::UnLoad(); // not load

	dir2 = dir1 + "/tmp";
	VERIFY(Library::Load(dir2.c_str()) == true);
	Library::UnLoad(); // all clear

	return true;
}

/////////////////////////////////////////////
//
// Factory Test
//
/////////////////////////////////////////////
bool test_1_7_1(void *This)
{
	TestSample *Test = (TestSample *)This;
	{
		FactoryTest::Result ret = FactoryTest::CreateFail();
		VERIFY(ret.Fail1 == NULL);
		VERIFY(ret.Fail2 == NULL);
		VERIFY(ret.GetInstance == NULL);
		VERIFY(ret.Destroy == false);
	}

	{
		FactoryTest::Result ret = FactoryTest::CreateSuccess();
		VERIFY(ret.Create != NULL);
		VERIFY(ret.GetInstance == ret.Create);
		VERIFY(ret.Fail1 == ret.Create);
		VERIFY(ret.Destroy == true);
	}

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
	Register("u1.1.1", test_1_1_1); // Observer
	Register("u1.2.1", test_1_2_1); // Thread
	Register("u1.2.2", test_1_2_2);
	Register("u1.3.1", test_1_3_1); // Timer
	Register("u1.3.2", test_1_3_2);

	Register("u1.4.1", test_1_4_1); // Config
	Register("u1.4.2", test_1_4_2);
	Register("u1.4.3", test_1_4_3);
	Register("u1.4.4", test_1_4_4);
	Register("u1.4.5", test_1_4_5);
	Register("u1.4.6", test_1_4_6);
	Register("u1.4.7", test_1_4_7);
	Register("u1.4.8", test_1_4_8);
	Register("u1.4.9", test_1_4_9);
	Register("u1.4.10", test_1_4_10);

	Register("u1.5.1", test_1_5_1); // String
	Register("u1.6.1", test_1_6_1); // Library
	Register("u1.7.1", test_1_7_1); // Factory

	return true;
}

const char *TestSample::OwnName(void)
{
	return "UtilsTest";
}

