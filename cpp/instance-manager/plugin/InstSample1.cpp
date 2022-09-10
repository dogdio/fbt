#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include "InstanceBase.h"

using namespace InstanceIF;

class InstSample1 : public InstanceBase {
public:
	void Initialize(void) override;
	void Finalize(void) override;
	void Start(void) override;
	void Stop(void) override;
	const char *OwnName(void) override;

	InstSample1() { AddBaseQueue(this); }
	~InstSample1() {}
};

namespace {
InstSample1 Inst;


} // namespace

void InstSample1::Initialize(void)
{
	std::thread thr ([](InstSample1 *This){
		usleep(1000*500);
		//sleep(4);
		printf("Initialize %s\n", This->OwnName());
		This->InitializeComplete();
	}, this);
	thr.detach();
}

void InstSample1::Finalize(void)
{
	std::thread thr ([](InstSample1 *This){
		usleep(1000*500);
		printf("Finalize %s\n", This->OwnName());
		This->FinalizeComplete();
	}, this);
	thr.detach();
}

void InstSample1::Start(void)
{
	std::thread thr ([](InstSample1 *This){
		usleep(1000*500);
		printf("Start %s\n", This->OwnName());
		This->StartComplete();
	}, this);
	thr.detach();
}

void InstSample1::Stop(void)
{
	std::thread thr ([](InstSample1 *This){
		usleep(1000*500);
		printf("Stop %s\n", This->OwnName());
		This->StopComplete();
	}, this);
	thr.detach();
}

const char *InstSample1::OwnName(void)
{
	return "InstSample1";
}

