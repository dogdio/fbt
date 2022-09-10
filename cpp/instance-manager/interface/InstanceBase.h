#ifndef IF_INSTANCE_BASE_H
#define IF_INSTANCE_BASE_H

namespace InstanceIF {

	class InstanceBase {
	public:
		InstanceBase();
		virtual ~InstanceBase();

		// implement by each instances
		virtual void Initialize(void) = 0;
		virtual void Finalize(void) = 0;
		virtual void Start(void) = 0;
		virtual void Stop(void) = 0;
		virtual const char *OwnName(void) = 0;
	
		// call once in constructor()
		void AddBaseQueue(InstanceBase *Base);

		// call by each instances when each process is completed
		void InitializeComplete(void);
		void FinalizeComplete(void);
		void StartComplete(void);
		void StopComplete(void);
	};
}

#endif
