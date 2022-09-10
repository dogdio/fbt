#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include "InstanceBase.h"

using namespace InstanceIF;

class InstSample3 : public InstanceBase {
public:
	void Initialize(void) override;
	void Finalize(void) override;
	void Start(void) override;
	void Stop(void) override;
	const char *OwnName(void) override;

	InstSample3() { AddBaseQueue(this); }
	~InstSample3() {}
};

namespace {
InstSample3 Inst;


} // namespace

void InstSample3::Initialize(void)
{
	printf("%s %s\n", __FUNCTION__, OwnName());
	InitializeComplete();
}

void InstSample3::Finalize(void)
{
	printf("%s %s\n", __FUNCTION__, OwnName());
	FinalizeComplete();
}

void InstSample3::Start(void)
{
	std::thread thr ([](InstSample3 *This){
		usleep(1000*200);
		printf("Start %s\n", This->OwnName());
		This->InitializeComplete();
	}, this);
	thr.detach();
}

void InstSample3::Stop(void)
{
	printf("%s %s\n", __FUNCTION__, OwnName());
	StopComplete();
}

const char *InstSample3::OwnName(void)
{
	return "InstSample3";
}

