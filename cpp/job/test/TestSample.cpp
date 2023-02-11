#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include "TestBase.h"

#include "Job.h"
#include "JobIF.h"
#include "Library.h"
#define MY_LOG_TYPE 0x2000
#include "Log.h"

using namespace test;
using namespace Utils;
using namespace Job;

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

TestSample Inst;

/////////////////// TestJob1 ////////////////////
class TestJob1 : public JobIF {
public:
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;
	bool IsSupported(void) override;
	bool IsAvailable(void) override;
	bool IsBusy(void) override;
	bool IsValidParam(JOBIF_ARGS &args) override;

	TestJob1(const char *job_id, const char *cmd_id) : JobIF(job_id, cmd_id) {}
	~TestJob1() {}
	bool Enqueue2(QUEUE_FUNC func);
	bool Supported = true;
	bool Available = true;
	bool Busy = false;
	int Counter = 0;
};

bool TestJob1::IsSupported(void)
{
	return Supported;
}

bool TestJob1::IsAvailable(void)
{
	return Available;
}

bool TestJob1::IsBusy(void)
{
	return Busy;
}

bool TestJob1::IsValidParam(JOBIF_ARGS &args)
{
	if(args.size() < 2)
		return false;
	if(args[0] == NULL)
		return false;
	if(args[1] == NULL)
		return false;

	return true;
}

JobIF::JOB_STATE TestJob1::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	auto func2 = [=](JobIF::JOB_STATE state) {
		Counter = 12345;
		func(JOB_STATE_COMPLETE);
	};

	if(strcmp(args[1], "abort") == 0) {
		return JOB_STATE_ABORT;
	}
	else if(strcmp(args[1], "exec-myself") == 0) {
		JOBIF_ARGS args2 = { "Command2", args[2] };
		if(strcmp(args[2], "callback") == 0) {
			return ExecMyself(args2, func2);
		}
		else {
			return ExecMyself(args2, NULL);
		}
	}
	else if(strcmp(args[1], "async") == 0) {
		Enqueue([=]{
			for(int i = 0; i < 100; i++) {
				Counter++;
				usleep(1000*10); // 10msec x 100
			}
			Busy = false;
			func(JOB_STATE_COMPLETE);
		});
		Busy = true;
		return JOB_STATE_PROCESSING;
	}
	else
		return JOB_STATE_COMPLETE;
}

bool TestJob1::Enqueue2(QUEUE_FUNC func)
{
	bool ret;
	const char *save = JobID;
	JobID = NULL; // GetJobID() will be failed
	ret = Enqueue(func);
	JobID = save;
	return ret;
}

class TestJob2 : public JobIF {
public:
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;
	bool IsSupported(void) override;
	bool IsAvailable(void) override;
	bool IsBusy(void) override;
	bool IsValidParam(JOBIF_ARGS &args) override;

	TestJob2(const char *job_id, const char *cmd_id) : JobIF(job_id, cmd_id) {}
	~TestJob2() {}
	bool Supported = true;
	bool Available = true;
	bool Busy = false;
	bool NotFound = false;
	int Counter = 0;
};

bool TestJob2::IsSupported(void)
{
	return Supported;
}

bool TestJob2::IsAvailable(void)
{
	return Available;
}

bool TestJob2::IsBusy(void)
{
	return Busy;
}

bool TestJob2::IsValidParam(JOBIF_ARGS &args)
{
	return true;
}

JobIF::JOB_STATE TestJob2::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	void *This = &Inst;
	TEST_LOG("param=%s", args[1]);

	if(NotFound)
		return JOB_STATE_NOT_FOUND;
	if(args.size() != 2)
		return JOB_STATE_INVALID_PARAM;
	if(args[0] == NULL)
		return JOB_STATE_INVALID_PARAM;
	if(args[1] == NULL)
		return JOB_STATE_INVALID_PARAM;

	if(strcmp(args[1], "Complete") == 0) {
		return JOB_STATE_COMPLETE;
	}
	else if(strcmp(args[1], "Abort") == 0) {
		return JOB_STATE_ABORT;
	}
	else if(strcmp(args[1], "callback") == 0) {
		TEST_LOG("func=%p", func);
		Enqueue([=]{
			for(int i = 0; i < 100; i++) {
				Counter++;
				usleep(1000*10); // 10msec x 100
			}
			Busy = false;
			func(JOB_STATE_COMPLETE);
		});
		Busy = true;
		return JOB_STATE_PROCESSING;
	}
	else
		return JOB_STATE_INVALID_PARAM;
}

TestJob1 JobCmd1("TestJob", "Command1");
TestJob2 JobCmd2("TestJob", "Command2");

/////////////////////////////////////////////////////////
//
// test code
//
/////////////////////////////////////////////////////////
bool test_1_5_1(void *This)
{
	VERIFY(JobCmd1.Enqueue2(nullptr) == false);
	return true;
}

bool test_1_4_2(void *This)
{
	Job::PublicIF *job;
	bool loop = true;
	JobIF::JOBIF_ARGS args = {"Command1", "async"};

	job = Job::Create("TestJob");
	VERIFY(job != NULL);

	JobCmd1.Counter = 0;
	VERIFY(job->Exec(args) == JOB_RET_PROCESSING);

	// no callback
	for(int i = 0; i < 3; i++) {
		usleep(1000*300); // 300msec
		VERIFY(job->Exec(args) == JOB_RET_BUSY);
	}
	usleep(1000*200);

	VERIFY(JobCmd1.Counter == 100);
	VERIFY(Job::Destroy("TestJob") == true);

	return true;
}

bool test_1_4_1(void *This)
{
	Job::PublicIF *job;
	bool loop = true;
	JOB_RET ret = JOB_RET_ABORT;
	JobIF::JOBIF_ARGS args = {"Command1", "async"};

	job = Job::Create("TestJob");
	VERIFY(job != NULL);

	auto func = [&](Job::JOB_CB_ARG &arg) {
		loop = false;
		ret = arg.ret;
		TEST_LOG("cmd=%s, ret=%s", arg.args[0], job->RetToStr(ret));
	};
	JobCmd1.Counter = 0;
	VERIFY(job->Exec(args, func) == JOB_RET_PROCESSING);

	while(1) {
		usleep(1000*300); // 300msec
		if(loop)
			VERIFY(job->Exec(args, func) == JOB_RET_BUSY);
		else
			break;
	}

	VERIFY(ret == JOB_RET_COMPLETE);
	VERIFY(JobCmd1.Counter == 100);
	VERIFY(Job::Destroy("TestJob") == true);

	return true;
}

bool test_1_3_2(void *This)
{
	Job::PublicIF *job;
	JobIF::JOBIF_ARGS args = {};
	bool loop = true;
	JOB_RET ret = JOB_RET_ABORT;

	job = Job::Create("TestJob");
	VERIFY(job != NULL);

	JobCmd1.Counter = 0;
	args = {"Command1", "exec-myself", "callback"};

	auto func = [&](Job::JOB_CB_ARG &arg) {
		TEST_LOG("cmd=%s, ret=%s", arg.args[0], job->RetToStr(ret));
		loop = false;
		ret = arg.ret;
	};
	JobCmd1.Counter = 0;
	JobCmd2.Counter = 0;

	VERIFY(job->Exec(args, func) == JOB_RET_PROCESSING);
	while(1) {
		usleep(1000*300); // 300msec
		if(loop)
			VERIFY(job->Exec(args, func) == JOB_RET_BUSY);
		else
			break;
	}

	VERIFY(JobCmd1.Counter == 12345);
	VERIFY(JobCmd2.Counter == 100);
	VERIFY(ret == JOB_RET_COMPLETE);

	VERIFY(Job::Destroy("TestJob") == true);
	return true;
}

bool test_1_3_1(void *This)
{
	Job::PublicIF *job;
	JobIF::JOBIF_ARGS args = {};

	job = Job::Create("TestJob");
	VERIFY(job != NULL);

	args = {"Command1", "exec-myself", "Complete"};

	JobCmd2.Supported = false;
	VERIFY(job->Exec(args) == JOB_RET_NOT_SUPPORTED);
	JobCmd2.Supported = true;

	JobCmd2.Available = false;
	VERIFY(job->Exec(args) == JOB_RET_UNAVAILABLE);
	JobCmd2.Available = true;

	JobCmd2.Busy = true;
	VERIFY(job->Exec(args) == JOB_RET_BUSY);
	JobCmd2.Busy = false;

	JobCmd2.NotFound = true;
	VERIFY(job->Exec(args) == JOB_RET_NOT_FOUND);
	JobCmd2.NotFound = false;

	VERIFY(job->Exec(args) == JOB_RET_COMPLETE);

	args = {"Command1", "exec-myself", "Abort"};
	VERIFY(job->Exec(args) == JOB_RET_ABORT);

	args = {"Command1", "exec-myself", "invalid-param"};
	VERIFY(job->Exec(args) == JOB_RET_INVALID_PARAM);

	VERIFY(Job::Destroy("TestJob") == true);

	return true;
}

bool test_1_2_1(void *This)
{
	Job::PublicIF *job;
	JobIF::JOBIF_ARGS args = {};

	job = Job::Create("TestJob");
	VERIFY(job != NULL);

	args = {};
	VERIFY(job->Exec(args) == JOB_RET_INVALID_PARAM);

	args = {"NotFound", "param1"};
	VERIFY(job->Exec(args) == JOB_RET_NOT_FOUND);

	args = {NULL, "param1"};
	VERIFY(job->Exec(args) == JOB_RET_NOT_FOUND);

	args = {"Command1", NULL };
	VERIFY(job->Exec(args) == JOB_RET_INVALID_PARAM);

	JobCmd1.Supported = false;
	args = {"Command1", "param1"};
	VERIFY(job->Exec(args) == JOB_RET_NOT_SUPPORTED);
	JobCmd1.Supported = true;

	JobCmd1.Available = false;
	args = {"Command1", "param1"};
	VERIFY(job->Exec(args) == JOB_RET_UNAVAILABLE);
	JobCmd1.Available = true;

	JobCmd1.Busy = true;
	args = {"Command1", "param1"};
	VERIFY(job->Exec(args) == JOB_RET_BUSY);
	JobCmd1.Busy = false;

	args = {"Command1", "abort"};
	VERIFY(job->Exec(args) == JOB_RET_ABORT);

	args = {"Command1", "param1"};
	VERIFY(job->Exec(args) == JOB_RET_COMPLETE);

	args = {"Command2", NULL};
	VERIFY(job->Exec(args) == JOB_RET_INVALID_PARAM);

	VERIFY(Job::Destroy("TestJob") == true);

	return true;
}

bool new_and_delete(const char *job_id, const char *cmd_id)
{
	TestJob1 *tj1;
	bool result;

	try {
		tj1 = new TestJob1(job_id, cmd_id);
		result = true;
	}
	catch (const std::exception & e) {
		result = false;
	}
	if(result)
		delete tj1;

	return result;
}

bool test_1_1_1(void *This)
{
	Job::PublicIF *job;
	Job::PublicIF *null;

	null = Job::GetInstance(NULL); // get null object

	// normal case
	job = Job::Create("TestJob");
	VERIFY(job != NULL);
	VERIFY(Job::GetInstance("TestJob") == job);
	TEST_LOG("job=%p", job);

	VERIFY(Job::Create("TestJob") == job); // already created
	VERIFY(Job::Destroy("TestJob") == true);

	// error case
	VERIFY(Job::GetInstance("TestJob") == null); // already deleted
	VERIFY(Job::Create(NULL) == null);
	VERIFY(Job::Destroy("NotCreated") == false);

	// null object is do nothing
	JobIF::JOBIF_ARGS args = { };
	VERIFY(null->Exec(args) == JOB_RET_COMPLETE);
	VERIFY_STR(null->RetToStr(JOB_RET_COMPLETE), "");

	VERIFY(new_and_delete(NULL, NULL) == false);
	VERIFY(new_and_delete("Hoge", NULL) == false);
	VERIFY(new_and_delete(NULL, "Hoge") == false);

	return true;
}

void TestSample::InitializeOnce(void)
{
	_TEST_LOG("START Once");
	Job::Init();

	Log::SetLevel(Log::TYPE_UTILS, Log::LEVEL_DBG);
	Log::SetLevel(MY_LOG_TYPE, Log::LEVEL_INFO);
	Log::SetString(MY_LOG_TYPE, "TEST");
	Log::SetToStdOut(false);
	Log::OpenFile("job-test.log");

//	JobCmd1 = new TestJob1("TestJob", "Command1");
//	JobCmd2 = new TestJob2("TestJob", "Command2");
}

void TestSample::FinalizeOnce(void)
{
	_TEST_LOG("STOP Once");
//	delete JobCmd1;
//	delete JobCmd2;
}

void TestSample::InitializePerTest(void)
{
	_TEST_LOG("TEST START %s", GetTestID());
	LOG_INFO(">>>>> %s start", GetTestID());
}

void TestSample::FinalizePerTest(void)
{
	_TEST_LOG("TEST STOP %s", GetTestID());
}

bool TestSample::RegisterTests(void)
{
	Register("1.1.1", test_1_1_1); // GetInstance/Create/Destroy
	Register("1.2.1", test_1_2_1); // Exec
	Register("1.3.1", test_1_3_1); // ExecMyself
	Register("1.3.2", test_1_3_2); // ExecMyself + async
	Register("1.4.1", test_1_4_1); // Exec async
	Register("1.4.2", test_1_4_2); // Exec async
	Register("1.5.1", test_1_5_1); // Enqueue

	return true;
}

const char *TestSample::OwnName(void)
{
	return "JobTest";
}

