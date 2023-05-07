#include <stdio.h>
#include <mutex>
#include <map>
#include <string>

#include "Job.h"
#include "JobIF.h"

#include "Thread.h"
#include "Lock.h"
#define MY_LOG_TYPE Job::LOG_TYPE
#include "Log.h"
#include "Factory.h"


using namespace Job;
using namespace Utils;
#define CLASS_ID "Job.Private."
#define ENUM_TO_STR(E) case E: return #E

typedef struct {
	Utils::Thread::ThreadIF *Tif;
} SharedMember;

using CMD_MAP = std::map<std::string, JobIF *>;
using JOB_MAP = std::map<std::string, CMD_MAP>;
using SHM_MAP = std::map<std::string, SharedMember *>;

namespace {
JOB_MAP JobMap;
SHM_MAP ShmMap;
std::mutex Mutex;

JobIF::JOB_STATE RetToState(JOB_RET ret)
{
	switch(ret) {
		case JOB_RET_COMPLETE:       return JobIF::JOB_STATE_COMPLETE;
		case JOB_RET_PROCESSING:     return JobIF::JOB_STATE_PROCESSING;
		case JOB_RET_NOT_FOUND:      return JobIF::JOB_STATE_NOT_FOUND;
		case JOB_RET_NOT_SUPPORTED:  return JobIF::JOB_STATE_NOT_SUPPORTED;
		case JOB_RET_BUSY:           return JobIF::JOB_STATE_BUSY;
		case JOB_RET_UNAVAILABLE:    return JobIF::JOB_STATE_UNAVAILABLE;
		case JOB_RET_INVALID_PARAM:  return JobIF::JOB_STATE_INVALID_PARAM;
		default:                     return JobIF::JOB_STATE_ABORT;
	}
}

JOB_RET StateToRet(JobIF::JOB_STATE state)
{
	switch(state) {
		case JobIF::JOB_STATE_COMPLETE:      return JOB_RET_COMPLETE;
		case JobIF::JOB_STATE_PROCESSING:    return JOB_RET_PROCESSING;
		case JobIF::JOB_STATE_NOT_FOUND:     return JOB_RET_NOT_FOUND;
		case JobIF::JOB_STATE_NOT_SUPPORTED: return JOB_RET_NOT_SUPPORTED;
		case JobIF::JOB_STATE_BUSY:          return JOB_RET_BUSY;
		case JobIF::JOB_STATE_UNAVAILABLE:   return JOB_RET_UNAVAILABLE;
		case JobIF::JOB_STATE_INVALID_PARAM: return JOB_RET_INVALID_PARAM;
		default:                             return JOB_RET_ABORT;
	}
}

bool SetSharedMember(const char *job_id, SharedMember *priv)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	try {
		ShmMap[job_id] = priv;
		ret = true;
		LOG_INFO("ShmMap insert: %d", ShmMap.size());
	}
	catch (const std::exception & e) {
		LOG_WARN("Insert Fail: job=%s, priv=%p", job_id, priv);
	}
	return ret;
}

void UnSetSharedMember(const char *job_id)
{
	Lock::LockIF lock(Mutex);

	ShmMap.erase(job_id);
	LOG_INFO("ShmMap erased: %d", ShmMap.size());
}

JobIF *GetJobIF(const char *job_id, const char *cmd_id)
{
	Lock::LockIF lock(Mutex);
	JobIF *ret = NULL;

	try {
		ret = JobMap.at(job_id).at(cmd_id);
	}
	catch (const std::exception & e) {
		LOG_WARN("Find Fail: job=%s, cmd=%s", job_id, cmd_id);
	}
	return ret;
}

// called by JobIF constructor
bool SetJobIF(const char *job_id, const char *cmd_id, JobIF *obj)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	try {
		JobMap[job_id][cmd_id] = obj;
		ret = true;
		LOG_INFO("JobMap insert: %d", JobMap.size());
	}
	catch (const std::exception & e) {
		LOG_WARN("Insert Fail: job=%s, cmd=%s, obj=%p", job_id, cmd_id, obj);
	}
	return ret;
}

void UnSetJobIF(const char *job_id, const char *cmd_id)
{
	JobMap.at(job_id).erase(cmd_id);
	if(JobMap.at(job_id).size() == 0) {
		JobMap.erase(job_id);
		LOG_INFO("JobMap erased: %d", JobMap.size());
	}
}

JOB_RET Prepare(JobIF *job, EXEC_ARGS &args)
{
	if(!job->IsSupported())
		return JOB_RET_NOT_SUPPORTED;

	if(!job->IsAvailable())
		return JOB_RET_UNAVAILABLE;

	if(job->IsBusy())
		return JOB_RET_BUSY;

	if(!job->IsValidParam(args))
		return JOB_RET_INVALID_PARAM;

	return JOB_RET_COMPLETE;
}

const char *ArgsToStr(std::string &str, EXEC_ARGS args)
{
	for(int i = 0; i < (int)args.size(); i++) {
		if(i > 0)
			str += ", ";
		if(args[i] != NULL)
			str += args[i];
	}
	return str.c_str();
}

} // anon

//////////////////////////////////////////
//
// JobIF
//
//////////////////////////////////////////
namespace Job {

JobIF::JobIF(const char *job_id, const char *cmd_id) : JobID(job_id), CmdID(cmd_id)
{
	if(!SetJobIF(job_id, cmd_id, this))
		throw std::length_error("job-map insert fail");
}

JobIF::~JobIF()
{
	UnSetJobIF(JobID, CmdID);
}

JobIF::JOB_STATE JobIF::ExecMyself(JOBIF_ARGS &args, JOBIF_CALLBACK func)
{
	JOB_RET ret;
	PublicIF *pub = GetInstance(GetJobID());

	auto func2 = [=](Job::JOB_CB_ARG &arg) {
		if(func != NULL)
			func(RetToState(arg.ret));
	};

	LOG_INFO("[%s] Exec pub: func=%p", JobID, func);
	ret = pub->Exec(args, func2);
	return RetToState(ret);
}

bool JobIF::Enqueue(QUEUE_FUNC func)
{
	SharedMember *priv = NULL;
	const char *job_id = GetJobID();

	{
		Lock::LockIF lock(Mutex);
		try {
			priv = ShmMap[job_id];
		}
		catch (const std::exception & e) {
			LOG_WARN("Insert Fail: job=%s, priv=%p", job_id, priv);
			return false;
		}
	}
	return priv->Tif->Enqueue(func);
}

} // Job


//////////////////////////////////////////
//
// JobPriv
//
//////////////////////////////////////////
class JobPriv : public PublicIF {
public:
	JobPriv(const char *name)
	{
		JobID = name;
		if(!SetSharedMember(JobID, &Shm))
			throw std::invalid_argument("name is null");

		ThreadName += name;

		Shm.Tif = Thread::Create(ThreadName.c_str());
	}

	~JobPriv() {
		Thread::Destroy(ThreadName.c_str());
		UnSetSharedMember(JobID);
	}

	JOB_RET Exec(EXEC_ARGS &args, CALLBACK func=NULL) override;
	const char *RetToStr(JOB_RET ret) override;

	const char *JobID;
	std::string ThreadName = CLASS_ID;
	SharedMember Shm;
};

JOB_RET JobPriv::Exec(EXEC_ARGS &args, CALLBACK func)
{
	JobIF *job;
	JOB_RET ret = JOB_RET_INVALID_PARAM;
	JobIF::JOB_STATE state;
	std::string tmp;

	// callback to APPL with job context
	auto async_cb = [=](JobIF::JOB_STATE state) {
		if(func == NULL)
			return;

		LOG_INFO("[%s] Enqueue: %s", JobID, args[0]);
		Shm.Tif->Enqueue([=](){
			std::string tmp2;
			JOB_RET ret2 = StateToRet(state);
			JOB_CB_ARG cb_arg = {args, ret2};

			LOG_INFO("[%s] Exec CB: args=%s", JobID, ArgsToStr(tmp2, args));
			func(cb_arg);
		});
	};

	if(args.size() <= 0)
		goto end;

	if((job = GetJobIF(JobID, args[0])) == NULL) {
		ret = JOB_RET_NOT_FOUND;
		goto end;
	}
	if((ret = Prepare(job, args)) != JOB_RET_COMPLETE)
		goto end;

	LOG_INFO("[%s] Exec: args=%s", JobID, ArgsToStr(tmp, args));
	state = job->Exec(args, async_cb);
	ret = StateToRet(state);
end:
	LOG_INFO("[%s] Exec: ret=%s", JobID, RetToStr(ret));
	return ret;
}

const char *JobPriv::RetToStr(JOB_RET ret)
{
	switch(ret) {
		ENUM_TO_STR(JOB_RET_NOT_FOUND);
		ENUM_TO_STR(JOB_RET_NOT_SUPPORTED);
		ENUM_TO_STR(JOB_RET_BUSY);
		ENUM_TO_STR(JOB_RET_UNAVAILABLE);
		ENUM_TO_STR(JOB_RET_INVALID_PARAM);
		ENUM_TO_STR(JOB_RET_COMPLETE);
		ENUM_TO_STR(JOB_RET_PROCESSING);
		default: return "JOB_RET_ABORT";
	}
}

/////////////////////////////////////////
//
// PublicIF
//
/////////////////////////////////////////
namespace {

Factory::FactoryIF<JobPriv, PublicIF> Inst(CLASS_ID);

class JobNull: public PublicIF {
public:
	JobNull(const char *name) { }
	~JobNull() { }
	JOB_RET Exec(EXEC_ARGS &args, CALLBACK func) { return JOB_RET_COMPLETE; }
	const char *RetToStr(JOB_RET ret) { return ""; }
};
JobNull JobNullInst("Job.Null");

} // anon

namespace Job {

PublicIF::PublicIF() {}
PublicIF::~PublicIF() {}

PublicIF *GetInstance(const char *name)
{
	PublicIF *pub = Inst.GetInstance(name);
	if(pub == NULL)
		return &JobNullInst;
	else
		return pub;
}

PublicIF *Create(const char *name)
{
	PublicIF *pub = Inst.Create(name);
	if(pub == NULL)
		return &JobNullInst;
	else
		return pub;
}

bool Destroy(const char *name)
{
	return Inst.Destroy(name);
}

void Init(void)
{
	Log::SetLevel(MY_LOG_TYPE, Log::LEVEL_INFO);
	Log::SetString(MY_LOG_TYPE, "JOB ");
}

} // Job

