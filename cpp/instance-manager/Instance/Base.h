#ifndef INSTANCE_BASE_H
#define INSTANCE_BASE_H

namespace Instance {
namespace Base {
	void StartWDT(void);
	void StopWDT(void);

	bool InitializeBase(void);
	void FinalizeBase(void);

	void Initialize(void);
	void Finalize(void);
	void Start(void);
	void Stop(void);
}
}

#endif
