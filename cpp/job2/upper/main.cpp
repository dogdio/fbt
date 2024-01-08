#include <iostream>
#include <sstream>
#include <string>

#include "JobIF.h"
#include "JobFactory.h"
#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"

using namespace Utils;
using namespace Job;

void ExecJob(std::string str, JobIF::JOBIF_ARGS ja)
{
	std::shared_ptr<JobIF> job;

	// 動的に確保されるので同じ処理を繰り返すことができる
	if((job = Factory::GetInstance(str)) != nullptr) {

		job->Exec(ja, [=](JobIF::JOB_STATE state) {
			std::cout << "Job Busy: " << job->IsBusy() << std::endl;
			std::cout << "Job END: " << state << std::endl;
		});
	}
}

int main(int argc, char **argv)
{
	Log::SetLevel(MY_LOG_TYPE, Log::LEVEL_INFO);
	Log::SetString(MY_LOG_TYPE, "JOB");

	std::string line;
	while(std::getline(std::cin, line)) {
		if(line == "q") break;

		std::istringstream iss(line);
		std::string token;
		std::string str;
		JobIF::JOBIF_ARGS tokens;

		if(iss >> token) {
			str = token;

			while(iss >> token)
				tokens.push_back(token);

			LOG_INFO("exec: >>>>> %s", line.c_str());
			ExecJob(str, tokens);
		}
	}

	return 0;
}
