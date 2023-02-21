#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <thread>
#include <vector>
#include <cstring>

#include "Lock.h"
#include "Timer.h"
#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"

using namespace Utils;

namespace {

typedef struct {
	const char *Name;
	Timer::FUNC_TYPE Func;
	uint32_t Repeat;
	uint32_t Timeout; // msec
	uint32_t TickCount; //usec
} TIMER_DATA;

using TIMER_VEC = std::vector<TIMER_DATA>;

class TimerPriv : public Timer::TimerIF {
public:
	TimerPriv(const char *Name);
	~TimerPriv();

	bool Add(const char *Name, Timer::FUNC_TYPE Func, uint32_t Timeout, uint32_t Repeat=0) override;
	bool Remove(const char *Name) override;
	bool Start(void) override;
	bool Stop(void) override;
	bool SetTick(uint32_t Tick) override;

	bool IsValidTick(uint32_t Tick);

	std::thread *ThreadInstance;
	const char *MyName;
	TIMER_VEC TimerVec;
	bool Run;
	uint32_t MyTick; //usec
	std::mutex Mutex;
};
Factory::FactoryIF<TimerPriv, Timer::TimerIF> Inst("Utils.Timer");

void SetNextTick(TimerPriv *This, struct timespec &tim)
{
	tim.tv_sec  += This->MyTick / 1000000; // usec --> sec
	tim.tv_nsec += This->MyTick * 1000; // usec --> nsec
	if(tim.tv_nsec >= 1000000000) {
		tim.tv_nsec -= 1000000000;
		tim.tv_sec++;
	}
}

void TimerCheck(TimerPriv *This)
{
	Lock::LockIF lock(This->Mutex);

	for(auto it = This->TimerVec.begin(); it != This->TimerVec.end(); ) {
		it->TickCount += This->MyTick;
		//printf("%d %d\n", it->TickCount, it->Timeout*1000);
		if(it->TickCount < it->Timeout * 1000) { // compare at usec
			it++;
			continue;
		}

		// timer occur
		it->Func();
		it->TickCount = 0;
		if(it->Repeat == 0) {
			it++;
			continue;
		}

		it->Repeat--;
		if(it->Repeat == 0)
			it = This->TimerVec.erase(it);
		else
			it++;
	}
}

void MainFunc(TimerPriv *This)
{
	struct timespec req = {};
	clock_gettime(CLOCK_REALTIME, &req);
	SetNextTick(This, req);

	while (This->Run) {
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &req, NULL);
		SetNextTick(This, req);

		TimerCheck(This);
	}
}

} // anonymous

bool TimerPriv::IsValidTick(uint32_t Tick)
{
	Lock::LockIF lock(Mutex);

	if(Tick == 0)
		return false;

	for(auto it: TimerVec) {
		if(it.Timeout < Tick/1000)
			return false;
	}
	return true;
}

bool TimerPriv::Add(const char *Name, Timer::FUNC_TYPE Func, uint32_t Timeout, uint32_t Repeat)
{
	Lock::LockIF lock(Mutex);
	TIMER_DATA td = {};

	if(Func == NULL)
		return false;
	if(Timeout < MyTick/1000)
		return false;

	td.Name = Name;
	td.Func = Func;
	td.Repeat = Repeat;
	td.Timeout = Timeout;
	td.TickCount = 0;
	TimerVec.push_back(td);
	return true;
}

bool TimerPriv::Remove(const char *Name)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	for(auto it = TimerVec.begin(); it != TimerVec.end(); ) {
		if(strcmp(Name, it->Name) == 0) {
			it = TimerVec.erase(it);
			ret = true;
			break;
		}
		else
			it++;
	}

	return ret;
}

bool TimerPriv::Start(void)
{
	if(Run)
		return false;

	Run = true;
	ThreadInstance = new std::thread(MainFunc, this);

	return true;
}

bool TimerPriv::Stop(void)
{
	if(!Run)
		return false;

	Run = false;
	ThreadInstance->join();
	LOG_DBG("%s END: %p", __func__, this->ThreadInstance);
	delete ThreadInstance;
	ThreadInstance = NULL;

	return true;
}

bool TimerPriv::SetTick(uint32_t Tick)
{
	if(Run)
		return false;
	if(!IsValidTick(Tick))
		return false;

	MyTick = Tick;
	return true;
}

TimerPriv::TimerPriv(const char *Name)
{
	if(Name == NULL)
		throw std::invalid_argument("Name is null");

	MyName = Name;
	ThreadInstance = NULL;
	Run = false;
	MyTick = 1000*100; // 100msec
}

TimerPriv::~TimerPriv()
{

}

namespace Utils {
namespace Timer {

class TimerNull : public Timer::TimerIF {
public:
	~TimerNull() { }
	TimerNull() { }
	bool SetTick(uint32_t Tick) { return true; }
	bool Add(const char *Name, FUNC_TYPE Func, uint32_t Timeout, uint32_t Repeat=0) { return true; }
	bool Remove(const char *Name) { return true; }
	bool Start(void) { return true; }
	bool Stop(void) { return true; }
};

TimerNull TimerNullInst;

TimerIF::TimerIF() {}
TimerIF::~TimerIF() {}

TimerIF *GetInstance(const char *Name)
{
	TimerIF *tif = Inst.GetInstance(Name);
	if(tif == NULL)
		return &TimerNullInst;
	else
		return tif;
}

TimerIF *Create(const char *Name)
{
	TimerIF *tif = Inst.Create(Name);
	if(tif == NULL)
		return &TimerNullInst;
	else
		return tif;
}

bool Destroy(const char *Name)
{
	return Inst.Destroy(Name);
}

} // Timer
} // Utils

