#include <stdio.h>
#include <unistd.h>
#include <string>
#include <thread>

#include "JobIF.h"
#define MY_LOG_TYPE 0x2000
#include "Log.h"

using namespace Job;
using namespace Utils;

class JobSample2a : public JobIF {
public:
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;

	JobSample2a(const char *job_id, const char *cmd_id) : JobIF(job_id, cmd_id) {}
	~JobSample2a() {}
};

JobIF::JOB_STATE JobSample2a::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	LOG_INFO("exec_async start: <<<<< %s, %s", GetCmdID(), args[1]);
	Enqueue([=](){
		sleep(3);				
		LOG_INFO("exec_async stop: <<<<< %s, %s", GetCmdID(), args[1]);
		JOBIF_ARGS ja1 = {"Command2", "XXX", "YYY"};
		ExecMyself(ja1, func);
	});

	return JOB_STATE_PROCESSING;
}

class JobSample2b : public JobIF {
public:
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;

	JobSample2b(const char *job_id, const char *cmd_id) : JobIF(job_id, cmd_id) {}
	~JobSample2b() {}
};

JobIF::JOB_STATE JobSample2b::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	LOG_INFO("invoke myself: +++++ %s, %s, %s", GetCmdID(), args[1], args[2]);
	func(JOB_STATE_COMPLETE);
	LOG_INFO("invoke myself: +++++ %s, %s, %s", GetCmdID(), args[1], args[2]);
	return JOB_STATE_COMPLETE;
}

namespace {
	JobSample2a JobInst1("SampleJob2", "Command1");
	JobSample2b JobInst2("SampleJob2", "Command2");
}

