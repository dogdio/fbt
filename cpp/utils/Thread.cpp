#include <stdio.h>
#include <sstream>
#include <thread>

#include "Thread.h"
#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"
#include "Queue.h"

using namespace Utils;

namespace {

typedef struct {
	Thread::FUNC_TYPE Func;
} QUEUE_DATA;

class ThreadPriv : public Thread::ThreadIF {
public:
	~ThreadPriv();
	ThreadPriv(const char *Name);

	bool Enqueue(Thread::FUNC_TYPE Func) override;

	std::thread *ThreadInstance;
	const char *MyName;
	Queue::QueueIF<QUEUE_DATA> Jobs;
};
Factory::FactoryIF<ThreadPriv, Thread::ThreadIF> Inst("Utils.Thread");

void MainFunc(ThreadPriv *This)
{
	QUEUE_DATA qd;

	LOG_WARN("TID=%llx, %s START:", Thread::GetID(), This->MyName);
	while(1)
	{
		qd = This->Jobs.WaitMsg();

		if(qd.Func == NULL)
			break;

		qd.Func();
	}
	LOG_WARN("TID=%llx, %s END:", Thread::GetID(), This->MyName);
}

}; // anonymous


bool ThreadPriv::Enqueue(Thread::FUNC_TYPE Func)
{
	if(Func == NULL)
		return false;

	QUEUE_DATA qd = {Func};
	Jobs.WakeMsg(qd);

	return true;
}

ThreadPriv::ThreadPriv(const char *Name)
{
	if(Name == NULL)
		throw std::invalid_argument("Name is null");

	ThreadInstance = new std::thread(MainFunc, this);
	MyName = Name;
}

ThreadPriv::~ThreadPriv()
{
	QUEUE_DATA qd = {NULL};
	Jobs.WakeMsg(qd);

	ThreadInstance->join();
	LOG_DBG("%s END: %p", __func__, this->ThreadInstance);
	delete ThreadInstance;
}

namespace Utils { // static functions
namespace Thread {

ThreadIF::ThreadIF() {}
ThreadIF::~ThreadIF() {}

ThreadIF *GetInstance(const char *Name)
{
	return Inst.GetInstance(Name);
}

ThreadIF *Create(const char *Name)
{
	return Inst.Create(Name);
}

bool Destroy(const char *Name)
{
	return Inst.Destroy(Name);
}

uint64_t GetID(void)
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	uint64_t Tid = std::stoull(ss.str());
	return Tid;
}

void Initialize(void)
{
	Inst.Create(Thread::VIEW);
	Inst.Create(Thread::MODEL);
	Inst.Create(Thread::CONTROLLER);
}

void Finalize(void)
{
	Inst.Destroy(Thread::VIEW);
	Inst.Destroy(Thread::MODEL);
	Inst.Destroy(Thread::CONTROLLER);
}

} // Thread
} // Utils

