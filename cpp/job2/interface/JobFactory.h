#ifndef JOB_FACTORY_H
#define JOB_FACTORY_H

#include <memory>

namespace Job {
namespace Factory {

	std::shared_ptr<JobIF> GetInstance(std::string job_id);

} // Factory
} // Job

#endif
