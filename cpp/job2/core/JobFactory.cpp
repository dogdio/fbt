#include <stdio.h>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include "JobIF.h"
#include "JobFactory.h"

#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"

#include "JobSample1.h"
#include "JobSample2.h"

using namespace Job;
using namespace Utils;

namespace Job {
namespace Factory {

using FACTORY_FUNC = std::function<JobIF *()>;
using JOB_MAP = std::map<std::string, FACTORY_FUNC>;

const JOB_MAP JobMap = {
	{ "JobSample1", [](){ return new JobSample1(); } },
	{ "JobSample2", [](){ return new JobSample2(); } },
};

// shared_ptr を返す場合は NullObject は使えない
// client は null チェックが必要
std::shared_ptr<JobIF> GetInstance(std::string job_id)
{
	LOG_INFO("Get: job=%s", job_id.c_str());

	try {
		if(JobMap.at(job_id)) {
			JobIF *job = JobMap.at(job_id)();
			std::shared_ptr<JobIF> sp(job);
			return sp;
		}
	}
	catch (const std::exception & e) {
		LOG_WARN("Find Fail: job=%s", job_id.c_str());
	}
	return nullptr;
}


} // Factory
} // Job
