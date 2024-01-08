#include <stdio.h>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include "JobIF.h"
#include "JobFactory.h"

#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"

using namespace Job;
using namespace Utils;

namespace Job {

class JobNull: public JobIF {
public:
	JobNull() {}
	~JobNull() {}
	JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) { return JOB_STATE_COMPLETE; }
};
JobNull JobNullInst;

} //Job

namespace Job {
namespace Factory {

using JOB_MAP = std::map<std::string, JobIF *>;
JOB_MAP JobMap = {};

// 各 Job から Factory に設定される。Factory は under に依存しない
bool SetInstance(std::string job_id, JobIF *job)
{
	try {
		JobMap[job_id] = job;
		return true;
	}
	catch (const std::exception & e) {
		LOG_WARN("Insert Fail: job=%s", job_id.c_str());
		return false;
	}
}

// 通常のポインタであれば NullObject が使える
JobIF *GetInstance(std::string job_id)
{
	LOG_INFO("Get: job=%s", job_id.c_str());

	try {
		if(JobMap.at(job_id))
			return JobMap[job_id];
	}
	catch (const std::exception & e) {
		LOG_WARN("Find Fail: job=%s", job_id.c_str());
	}
	return &JobNullInst;
}


} // Factory
} // Job
