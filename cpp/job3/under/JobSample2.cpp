#include <string>
#include <thread>
#include <unistd.h>
#include "JobIF.h"
#include "JobFactory.h"

#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"

#include "JobSample2.h"

using namespace Job;
using namespace Utils;

JobSample2::JobSample2()
{
	Factory::SetInstance("JobSample2", this);
	LOG_INFO("INIT: %s", __func__);
	busy = false;
}

JobSample2::~JobSample2()
{
	LOG_INFO("EXIT: %s", __func__);
}

bool JobSample2::IsBusy(void)
{
	return busy;
}

JobIF::JOB_STATE JobSample2::Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	for(auto a : args)
		LOG_INFO("Arg: %s", a.c_str());

	std::thread thr([=]() {
		sleep(3);
		busy = false;
		func(JOB_STATE_COMPLETE);
	});
	thr.detach();
	busy = true;
	return JOB_STATE_PROCESSING;
}

JobSample2 Job2;
