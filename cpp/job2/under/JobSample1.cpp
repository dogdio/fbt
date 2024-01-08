#include <string>
#include "JobIF.h"

#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"

#include "JobSample1.h"

using namespace Job;
using namespace Utils;

JobSample1::JobSample1()
{
	LOG_INFO("INIT: %s", __func__);
}

JobSample1::~JobSample1()
{
	LOG_INFO("EXIT: %s", __func__);
}

JobIF::JOB_STATE JobSample1::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	for(auto a : args)
		LOG_INFO("Arg: %s", a.c_str());

	return JOB_STATE_COMPLETE;
}


