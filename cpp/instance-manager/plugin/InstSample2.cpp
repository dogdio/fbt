#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include "InstanceBase.h"

using namespace InstanceIF;

class InstSample2 : public InstanceBase {
public:
	void Initialize(void) override;
	void Finalize(void) override;
	void Start(void) override;
	void Stop(void) override;
	const char *OwnName(void) override;

	InstSample2() { AddBaseQueue(this); }
	~InstSample2() {}
};

namespace {
InstSample2 Inst;


} // namespace

void InstSample2::Initialize(void)
{
	std::thread thr ([](InstSample2 *This){
		usleep(1000*400);
		printf("Initialize %s\n", This->OwnName());
		This->InitializeComplete();
	}, this);
	thr.detach();
}

void InstSample2::Finalize(void)
{
	printf("%s %s\n", __FUNCTION__, OwnName());
	FinalizeComplete();
}

void InstSample2::Start(void)
{
	std::thread thr ([](InstSample2 *This){
		usleep(1000*400);
		printf("Start %s\n", This->OwnName());
		This->InitializeComplete();
	}, this);
	thr.detach();
}

void InstSample2::Stop(void)
{
	printf("%s %s\n", __FUNCTION__, OwnName());
	StopComplete();
}

const char *InstSample2::OwnName(void)
{
	return "InstSample2";
}

