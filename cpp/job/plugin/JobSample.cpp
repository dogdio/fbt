#include <stdio.h>
#include <string>
#include "JobIF.h"

#define MY_LOG_TYPE Log::TYPE_ANY1
#include "Log.h"

using namespace Job;
using namespace Utils;

class JobSample1a : public JobIF {
public:
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;

	JobSample1a(const char *job_id, const char *cmd_id) : JobIF(job_id, cmd_id) {}
	~JobSample1a() {}
};

JobIF::JOB_STATE JobSample1a::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	LOG_INFO("exec: >>>>> %s, %s %s", GetCmdID(), args[1], args[2]);
	return JOB_STATE_COMPLETE;
}


class JobSample1b : public JobIF {
public:
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;

	JobSample1b(const char *job_id, const char *cmd_id) : JobIF(job_id, cmd_id) {}
	~JobSample1b() {}
};

JobIF::JOB_STATE JobSample1b::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	LOG_INFO("exec: >>>>> %s, %s %s", GetCmdID(), args[1], args[2]);
	return JOB_STATE_COMPLETE;
}

namespace {
	JobSample1a JobCmd1("SampleJob1", "Command1");
	JobSample1b JobCmd2("SampleJob1", "Command2");
}

