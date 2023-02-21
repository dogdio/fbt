#ifndef UTILS_INSTANCES_H
#define UTILS_INSTANCES_H

#include <mutex>
#include <map>

#include "Lock.h"

namespace Utils {
namespace Instances {

	template <typename Parent_t>
	class InstancesIF final {

	public:
		InstancesIF(const char *name) : MyName(name) { }
		~InstancesIF() { }
		InstancesIF (const InstancesIF& rv) = delete;
		InstancesIF (const InstancesIF&& rv) = delete;
		InstancesIF& operator=(const InstancesIF& rv) = delete;
		InstancesIF& operator=(const InstancesIF&& rv) = delete;

		using FUNC_TYPE = std::function<void(const char *Name, Parent_t *Obj)>;

		Parent_t *Get(const char *Name)
		{
			Lock::LockIF lock(Mutex);

			return GetInstancePriv(Name);
		}

		void UnSet(const char *Name)
		{
			Lock::LockIF lock(Mutex);

			if(GetInstancePriv(Name) != NULL)
				InstanceMap.erase(Name);
			LOG_DBG("[%s] UnSet: %s", MyName, Name);
		}

		void Set(const char *Name, Parent_t *arg)
		{
			Lock::LockIF lock(Mutex);

			if(GetInstancePriv(Name) == NULL)
				InstanceMap[Name] = arg;
			LOG_DBG("[%s] Set: %s => %p", MyName, Name, arg);
		}

		void Foreach(FUNC_TYPE Func)
		{
			Lock::LockIF lock(Mutex);

			if(Func == NULL)
				return;
    
			for(auto it: InstanceMap) {
				Func(it.first.c_str(), it.second);
			}
		}

	private:
		Parent_t *GetInstancePriv(const char *Name)
		{
			try {
				return InstanceMap.at(Name);
			}
			catch (const std::exception & e){
				LOG_DBG("[%s] not found: %s", MyName, Name);
				return NULL;
			}
		}

		using INST_MAP = std::map<std::string, Parent_t *>;
		INST_MAP InstanceMap;
		std::mutex Mutex;
		const char *MyName;
	};

} // Instances
} // Utils

#endif
