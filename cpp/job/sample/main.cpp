#include <stdio.h>
#include <unistd.h>

#include "Job.h"
#include <string>
#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"
#include "Library.h"

using namespace Utils;

//
// +--- SampleJob1 --+  +--- SampleJob2 --+
// |                 |  |                 |
// |    Command1     |  |    Command1     |
// |    Command2     |  |    Command2     |
// |                 |  |                 |
// +-----------------+  +-----------------+
//
bool loop;
void async(void)
{
	Job::EXEC_ARGS ja1 = {"Command1", "##call", "##async" };
	Job::PublicIF *job = Job::GetInstance("SampleJob2");
	loop = true;

	auto func = [&](Job::JOB_CB_ARG &arg) {
		Job::PublicIF *job = Job::GetInstance("SampleJob2");
		LOG_INFO_CYAN("** ret=%s(%d), %s => %s %s",
		 	job->RetToStr(arg.ret), arg.ret, arg.args[0], arg.args[1], arg.args[2]);
		loop = false;
	};

	job->Exec(ja1, func);
}

void wait(void)
{
	while(loop) {
		LOG_INFO_GREEN("## wait");
		sleep(1);
	}
}

int main(int argc, char **argv)
{
	Job::PublicIF *job;

	Job::Init();

	// Command instances are generated(JobSample.cpp, JobSample2.cpp)
	Library::Load("./plugin");

	Job::Create("SampleJob1");
	Job::Create("SampleJob2");

	job = Job::GetInstance("SampleJob1");

	Job::EXEC_ARGS ja1 = {"Command1", "aaa", "bbb"};
	job->Exec(ja1);
	Job::EXEC_ARGS ja2 = {"Command2", "ccc", "ddd"};
	job->Exec(ja2);

	async();
	wait();

	Job::Destroy("SampleJob1");
	Job::Destroy("SampleJob2");
	Library::UnLoad();
	return 0;
}
