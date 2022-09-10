#include <stdio.h>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <unistd.h>

#include "Thread.h"
#include "Event.h"
using namespace Utils;

#include "InstanceBase.h"
using namespace InstanceIF;
#include "Base.h"
using namespace Instance;

namespace {
	
typedef struct {
	InstanceBase *Base;
} SharedData;
using SHARED_DATA = std::shared_ptr<SharedData>;
using BASE_MAP = std::map<InstanceBase *, bool>;

std::vector<InstanceBase *> VecInstances;
BASE_MAP ProcessingList;
Event::EventIF<bool> eif;
Thread::ThreadIF *tif = NULL;
const char *ThreadName = "Instance.Base";
const char *CurrentProcess = "";
bool WdtRun = false;

void Complete(InstanceBase *Base)
{
	SHARED_DATA buf= std::make_shared<SharedData>();
	buf->Base = Base;

	auto Func = [=](void) {
		bool msg = true;
    
		if(!ProcessingList.at(buf->Base))
			return;

		ProcessingList.erase(buf->Base);
    
		if(ProcessingList.empty())
			eif.WakeMsg(msg);
	};
	tif->Enqueue(Func);
}

void Prepare(const char *Name)
{
	CurrentProcess = Name;
	ProcessingList = {};

	for(auto Inst : VecInstances)
		ProcessingList[Inst] = true;
}

};

//
// InstanceBase Common Function: for user code
//
InstanceBase::InstanceBase()
{

}

InstanceBase::~InstanceBase()
{

}

void InstanceBase::AddBaseQueue(InstanceBase *Base)
{
	VecInstances.push_back(Base);
}

void InstanceBase::InitializeComplete(void)
{
	Complete(this);
}

void InstanceBase::FinalizeComplete(void)
{
	Complete(this);
}

void InstanceBase::StartComplete(void)
{
	Complete(this);
}

void InstanceBase::StopComplete(void)
{
	Complete(this);
}

//
// InstanceBase API: for main
//
namespace Instance {
namespace Base {

void StartWDT(void)
{
	if(WdtRun)
		return;

	WdtRun = true;
	std::thread thr ([]{
		auto Func = []() {
			//printf("!!\n");
			if(ProcessingList.empty())
				return;
			if(!WdtRun)
				return;
    
			int num = 0;
			for(auto Proc : ProcessingList)
				printf("WDT[%d] still %s: %s\n", num++, CurrentProcess, Proc.first->OwnName());
		};

		while(WdtRun) {
			tif->Enqueue(Func);
			sleep(1);
		}
	});
	thr.detach();
}

void StopWDT(void)
{
	WdtRun = false;
}

bool InitializeBase(void)
{
	bool ret = true;
	tif = Thread::Create(ThreadName);
	if(tif == NULL)
		ret = false;

	return ret;
}

void FinalizeBase(void)
{
	Thread::Destroy(ThreadName);
}

void Initialize(void)
{
	// Initialize
	Prepare("Initializing");
	for(auto Inst : VecInstances)
		Inst->Initialize();
	eif.WaitMsg();
}

void Finalize(void)
{
	// Finalize
	Prepare("Finalizing");
	for(auto Inst : VecInstances)
		Inst->Finalize();
	eif.WaitMsg();
}

void Start(void)
{
	// Start
	Prepare("Starting");
	for(auto Inst : VecInstances)
		Inst->Start();
	eif.WaitMsg();
}

void Stop(void)
{
	// Stop
	Prepare("Stopping");
	for(auto Inst : VecInstances)
		Inst->Stop();
	eif.WaitMsg();
}

} // Base
} // Instance

