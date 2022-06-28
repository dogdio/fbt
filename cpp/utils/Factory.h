#ifndef UTILS_FACTORY_H
#define UTILS_FACTORY_H

#include <mutex>
#include <map>

#include "Lock.h"

namespace Utils {
namespace Factory {

	template <typename Child_t, typename Parent_t>
	class FactoryIF final {

	public:
		FactoryIF(const char *name) : MyName(name) { }
		~FactoryIF() { }
		FactoryIF (const FactoryIF& rv) = delete;
		FactoryIF (const FactoryIF&& rv) = delete;
		FactoryIF& operator=(const FactoryIF& rv) = delete;
		FactoryIF& operator=(const FactoryIF&& rv) = delete;

		Parent_t *GetInstance(const char *Name)
		{
			Lock::LockIF lock(Mutex);

			return GetInstancePriv(Name);
		}

		Parent_t *Create(const char *Name)
		{
			Lock::LockIF lock(Mutex);
			Child_t *c = NULL;

			if(GetInstancePriv(Name) != NULL)
				return NULL; // already exists

			try {
				c = new Child_t(Name);
			}
			catch (const std::exception & e){
				LOG_ERR("new() fail: %s", e.what());
				return NULL;
			}
			SetInstance(Name, c);
			return c;
		}

		bool Destroy(const char *Name)
		{
			Lock::LockIF lock(Mutex);
			bool ret = false;
			Parent_t *p = GetInstancePriv(Name);

			if(p != NULL) {
				UnSetInstance(Name);
				delete p;
				ret = true;
			}
			return ret;
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

		void UnSetInstance(const char *Name)
		{
			InstanceMap.erase(Name);
			LOG_DBG("[%s] UnSet: %s", MyName, Name);
		}

		void SetInstance(const char *Name, Parent_t *arg)
		{
			InstanceMap[Name] = arg;
			LOG_DBG("[%s] Set: %s => %p", MyName, Name, arg);
		}

		using INST_MAP = std::map<std::string, Parent_t *>;
		INST_MAP InstanceMap;
		std::mutex Mutex;
		const char *MyName;
	};

} // Factory
} // Utils

#endif
