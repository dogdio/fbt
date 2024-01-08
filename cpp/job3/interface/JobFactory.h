#ifndef JOB_FACTORY_H
#define JOB_FACTORY_H

#include <memory>

namespace Job {
namespace Factory {

	bool SetInstance(std::string job_id, JobIF *job);
	JobIF *GetInstance(std::string job_id);

} // Factory
} // Job

#endif
