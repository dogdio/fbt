#include <stdio.h>
#include <mutex>
#include <map>

#include "Observer.h"
#include "Lock.h"
#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"

using namespace Utils;

namespace {

using SUBSCRIBE_MAP = std::map<std::string, Observer::FUNC_TYPE>;

class ObserverPriv : public Observer::ObserverIF {
public:
	ObserverPriv(const char *Name)
	{
		if(Name == NULL)
			throw std::invalid_argument("Name is null");
		MyName = Name;
	} 
	~ObserverPriv() {}

	bool Subscribe(const char *Reader, Observer::FUNC_TYPE Func) override;
	bool UnSubscribe(const char *Reader) override;
	bool Notify(Observer::Msg_t &Msg) override;

	const char *MyName;
	SUBSCRIBE_MAP SubscribeMap;
	std::mutex Mutex;
};
Factory::FactoryIF<ObserverPriv, Observer::ObserverIF> Inst("Utils.Observer");

}; // anonymous

bool ObserverPriv::Subscribe(const char *Reader, Observer::FUNC_TYPE Func)
{
	Lock::LockIF lock(Mutex);

	SubscribeMap[Reader] = Func;
	LOG_DBG("[%s] Subscribe: %s => %p", MyName, Reader, Func);
	return true;
}

bool ObserverPriv::UnSubscribe(const char *Reader)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	if(SubscribeMap.erase(Reader) != 0) {
		ret = true;
		LOG_DBG("[%s] UnSubscribe: %s", MyName, Reader);
	}
	else {
		LOG_ERR("[%s] UnSubscribe Fail: %s", MyName, Reader);
	}
	return ret;
}

bool ObserverPriv::Notify(Observer::Msg_t &Msg)
{
	Lock::LockIF lock(Mutex);

	for(auto it : SubscribeMap) {
		if(it.second != NULL) {
			LOG_DBG("[%s] Notify to: %s", MyName, it.first.c_str());
			it.second(Msg);
		}
	}
	return true;
}

namespace Utils { // static functions
namespace Observer {

ObserverIF::ObserverIF() {}
ObserverIF::~ObserverIF() {}

ObserverIF *GetInstance(const char *Name)
{
	return Inst.GetInstance(Name);
}

ObserverIF *Create(const char *Name)
{
	return Inst.Create(Name);
}

bool Destroy(const char *Name)
{
	return Inst.Destroy(Name);
}

} // Observer
} // Utils

