#include <fstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

#include "Config.h"
#include "Lock.h"
#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"
#include "String.h"

using namespace Utils;

namespace {

typedef struct {
	int32_t init;
	int32_t min;
	int32_t max;
	int32_t curr;
} INT_VALUE;

typedef struct {
	float init;
	float min;
	float max;
	float curr;
} FLOAT_VALUE;

typedef struct {
	const char *init;
	const char *regex;
	int32_t length;
	std::string curr;
} STRING_VALUE;

typedef struct {
	const char *Receiver;
	Config::ConfigIF::FUNC_TYPE Func;
} SUBSCRIBE_INFO;

using CONFIG_STR = std::unordered_map<std::string, STRING_VALUE>;
using CONFIG_INT = std::unordered_map<std::string, INT_VALUE>;
using CONFIG_FLOAT = std::unordered_map<std::string, FLOAT_VALUE>;


class ConfigPriv : public Config::ConfigIF {

public:
	ConfigPriv(const char *Name);
	~ConfigPriv();

	bool Define(const char *Name, int32_t init, int32_t min, int32_t max) override;
	bool Define(const char *Name, float init, float min, float max) override;
	bool Define(const char *Name, const char *init, const char *regex, int32_t length) override;

	bool Set(const char *Name, const char *Value) override;
	bool Set(const char *Name, int32_t Value) override;
	bool Set(const char *Name, float Value) override;

	const char *GetString(const char *Name) override;
	int32_t GetInteger(const char *Name) override;
	float GetFloat(const char *Name) override;

	bool Reset(const char *Name) override;

	using SUBSCRIBE_VEC = std::vector<SUBSCRIBE_INFO>;
	using SUBSCRIBE_MAP = std::unordered_map<std::string, SUBSCRIBE_VEC>;
	bool Subscribe(const char *Name, const char *Receiver, FUNC_TYPE Func) override;
	bool UnSubscribe(const char *Name, const char *Receiver) override;

	bool Load(const char *FileName) override;
	bool Save(const char *FileName) override;
	void Dump(void) override;

	// Internal APIs
	template <typename Map_t, typename Value_t>
	bool SetAllType(Map_t &Map, const char *Name, Value_t Value);
	void Notify(const char *Name);
	void WriteToFile(FILE *fp, bool verbose);

	CONFIG_STR ConfigStr;
	CONFIG_INT ConfigInt;
	CONFIG_FLOAT ConfigFloat;
	SUBSCRIBE_MAP SubscribeMap;
	const char *MyName;
	std::mutex Mutex;
};
Factory::FactoryIF<ConfigPriv, Config::ConfigIF> Inst("Utils.Config");

bool IsValidSyntax(std::string str, int32_t length, const char *regex)
{
	std::smatch sm;

	if(str.length() > (long unsigned int)length)
		return false;
	if(!String::MatchAll(str, sm, regex))
		return false;

	return true;
}

template <typename Map_t>
bool SetInitValue(Map_t &Map, const char *Name)
{
	try {
		Map.at(Name);
		Map[Name].curr = Map[Name].init;
		return true;
	}
	catch (const std::exception & e) {
		return false;
	}
}

bool SetCurrentValue(CONFIG_STR &ConfigStr, const char *Name, const char *Value, bool &Modified)
{
	if(!IsValidSyntax(Value, ConfigStr[Name].length, ConfigStr[Name].regex))
		return false;

	if(ConfigStr[Name].curr != Value)
		Modified = true;
	ConfigStr[Name].curr = Value;
	return true;
}

template <typename Map_t, typename Type_t>
bool SetCurrentValue(Map_t &Map, const char *Name, Type_t Value, bool &Modified)
{
	if(Value < Map[Name].min)
		return false;
	if(Value > Map[Name].max)
		return false;

	if(Map[Name].curr != Value)
		Modified = true;
	Map[Name].curr = Value;
	return true;
}

bool CreateNewValue(CONFIG_STR &ConfigStr, const char *Name, const char *init, const char *regex, int32_t length)
{
	if(init == NULL)
		return false;
	if(regex == NULL)
		return false;
	if(!IsValidSyntax(init, length, regex))
		return false;

	try {
		ConfigStr[Name].init = init;
		ConfigStr[Name].curr = init;
		ConfigStr[Name].regex = regex;
		ConfigStr[Name].length = length;
		return true;
	}
	catch (const std::exception & e) {
		return false;
	}
}

template <typename Map_t, typename Type_t>
bool CreateNewValue(Map_t &Map, const char *Name, Type_t init, Type_t min, Type_t max)
{
	if(min > max)
		return false;
	if(init < min)
		return false;
	if(init > max)
		return false;

	try {
		Map[Name].init = Map[Name].curr = init;
		Map[Name].min = min;
		Map[Name].max = max;
		return true;
	}
	catch (const std::exception & e) {
		return false;
	}
}

template <typename Map_t>
bool IsValidName(Map_t &Map, const char *Name)
{
	try {
		Map.at(Name);
		return true;
	}
	catch (const std::exception & e){
		return false;
	}
}

} // anonymous

///////////////////////////////////////////////////////
//
// ConfigPriv impl
//
///////////////////////////////////////////////////////
ConfigPriv::ConfigPriv(const char *Name)
{
	if(Name == NULL)
		throw std::invalid_argument("Name is null");
	MyName = Name;
}

ConfigPriv::~ConfigPriv()
{

}

void ConfigPriv::WriteToFile(FILE *fp, bool verbose)
{
	fprintf(fp, "# [%s]\n", MyName);
	for(auto it : ConfigStr) {
		if(verbose)
			fprintf(fp, "\n# %s: Init=%s, Len=%d, Pattern=%s\n", it.first.c_str(), it.second.init, it.second.length, it.second.regex);
		fprintf(fp, "%s = %s\n", it.first.c_str(), it.second.curr.c_str());
	}
	for(auto it : ConfigInt) {
		if(verbose)
			fprintf(fp, "\n# %s: Init=%d, Min=%d, Max=%d\n", it.first.c_str(), it.second.init, it.second.min, it.second.max);
		fprintf(fp, "%s = %d\n", it.first.c_str(), it.second.curr);
	}
	for(auto it : ConfigFloat) {
		if(verbose)
			fprintf(fp, "\n# %s: Init=%f, Min=%f, Max=%f\n", it.first.c_str(), it.second.init, it.second.min, it.second.max);
		fprintf(fp, "%s = %f\n", it.first.c_str(), it.second.curr);
	}
}

bool ConfigPriv::Define(const char *Name, int32_t init, int32_t min, int32_t max)
{
	Lock::LockIF lock(Mutex);
	return CreateNewValue(ConfigInt, Name, init, min, max);
}

bool ConfigPriv::Define(const char *Name, float init, float min, float max)
{
	Lock::LockIF lock(Mutex);
	return CreateNewValue(ConfigFloat, Name, init, min, max);
}

bool ConfigPriv::Define(const char *Name, const char *init, const char *regex, int32_t length)
{
	Lock::LockIF lock(Mutex);
	return CreateNewValue(ConfigStr, Name, init, regex, length);
}

void ConfigPriv::Notify(const char *Name)
{
	ConfigPriv::SUBSCRIBE_VEC Vec;

	try {
		Vec = SubscribeMap.at(Name);
	}
	catch (const std::exception & e) {
		return;
	}

	for(auto it = Vec.begin(); it != Vec.end(); it++) {
		it->Func(this);
	}
}

template <typename Map_t, typename Value_t>
bool ConfigPriv::SetAllType(Map_t &Map, const char *Name, Value_t Value)
{
	bool Modified = false;
	{
		Lock::LockIF lock(Mutex);

		if(!IsValidName(Map, Name))
			return false;
		if(!SetCurrentValue(Map, Name, Value, Modified))
			return false;
	}
	if(Modified)
		Notify(Name);

	return true;
}

bool ConfigPriv::Set(const char *Name, const char *Value)
{
	return SetAllType(ConfigStr, Name, Value);
}

bool ConfigPriv::Set(const char *Name, int32_t Value)
{
	return SetAllType(ConfigInt, Name, Value);
}

bool ConfigPriv::Set(const char *Name, float Value)
{
	return SetAllType(ConfigFloat, Name, Value);
}

const char *ConfigPriv::GetString(const char *Name)
{
	Lock::LockIF lock(Mutex);

	if(!IsValidName(ConfigStr, Name))
		return "";
	else
		return ConfigStr[Name].curr.c_str();
}

int32_t ConfigPriv::GetInteger(const char *Name)
{
	Lock::LockIF lock(Mutex);

	if(!IsValidName(ConfigInt, Name))
		return 0;
	else
		return ConfigInt[Name].curr;
}

float ConfigPriv::GetFloat(const char *Name)
{
	Lock::LockIF lock(Mutex);

	if(!IsValidName(ConfigFloat, Name))
		return 0.0f;
	else
		return ConfigFloat[Name].curr;
}

bool ConfigPriv::Reset(const char *Name)
{
	Lock::LockIF lock(Mutex);

	if(Name == NULL) {
		for(auto it = ConfigStr.begin(); it !=ConfigStr.end(); it++)
			it->second.curr = it->second.init;
		for(auto it = ConfigInt.begin(); it !=ConfigInt.end(); it++)
			it->second.curr = it->second.init;
		for(auto it = ConfigFloat.begin(); it !=ConfigFloat.end(); it++)
			it->second.curr = it->second.init;
		return true;
	}
	else {
		if(SetInitValue(ConfigStr, Name))
			return true;
		if(SetInitValue(ConfigInt, Name))
			return true;
		if(SetInitValue(ConfigFloat, Name))
			return true;

		return false;
	}
}

bool ConfigPriv::Subscribe(const char *Name, const char *Receiver, FUNC_TYPE Func)
{
	Lock::LockIF lock(Mutex);

	if(Receiver == NULL)
		return false;
	if(Func == NULL)
		return false;

	SUBSCRIBE_INFO Info = { Receiver, Func };
	try {
		SubscribeMap[Name].push_back(Info);
		return true;
	}
	catch (const std::exception & e) {
		return false;
	}
}

bool ConfigPriv::UnSubscribe(const char *Name, const char *Receiver)
{
	Lock::LockIF lock(Mutex);

	if(Receiver == NULL)
		return false;

	try {
		SUBSCRIBE_VEC &Vec = SubscribeMap[Name];

		for(auto it = Vec.begin(); it != Vec.end(); it++) {
			if(strcmp(it->Receiver, Receiver) == 0) {
				Vec.erase(it);
				return true;
			}
		}
	}
	catch (const std::exception & e) {
		return false;
	}
	return false;
}

bool ConfigPriv::Load(const char *FileName)
{
	Lock::LockIF lock(Mutex);
	std::ifstream ifs(FileName);
	std::string str;
	std::smatch sm;
	bool Modified;

	if(ifs.fail())
		return false;

	while(getline(ifs, str)) {
		String::Replace(str, "#.*", "");

		if(String::MatchAll(str, sm, "^(.*)=(.*)$")) {
			std::string name = sm[1];
			std::string value = sm[2];
			bool ret = false;

			String::Replace(name, "^ +| +$", "");
			String::Replace(value, "^ +| +$", "");

			if(IsValidName(ConfigStr, name.c_str())) // String
				ret = SetCurrentValue(ConfigStr, name.c_str(), value.c_str(), Modified);
			else if(IsValidName(ConfigInt, name.c_str())) // Integer
				ret = SetCurrentValue(ConfigInt, name.c_str(), atoi(value.c_str()), Modified);
			else if(IsValidName(ConfigFloat, name.c_str())) // Float
				ret = SetCurrentValue(ConfigFloat, name.c_str(), (float)atof(value.c_str()), Modified);

			LOG_DBG("Load ret=%d, [%s] => %s", ret, name.c_str(), value.c_str());
		}
	}
	return true;
}

bool ConfigPriv::Save(const char *FileName)
{
	Lock::LockIF lock(Mutex);
	FILE *fp = fopen(FileName, "w");
	if(fp == NULL)
		return false;

	WriteToFile(fp, true);
	fclose(fp);

	return true;
}

void ConfigPriv::Dump(void)
{
	Lock::LockIF lock(Mutex);
	WriteToFile(stdout, false);
}

namespace Utils {
namespace Config {

class ConfigNull : public Config::ConfigIF {
public:
	~ConfigNull() { }
	ConfigNull() { }
	bool Define(const char *Name, int32_t init, int32_t min, int32_t max) { return true; }
	bool Define(const char *Name, float init, float min, float max) { return true; }
	bool Define(const char *Name, const char *init, const char *regex, int32_t length) { return true; }
	bool Set(const char *Name, const char *Value) { return true; }
	bool Set(const char *Name, int32_t Value) { return true; }
	bool Set(const char *Name, float Value) { return true; }
	const char *GetString(const char *Name) { return ""; }
	int32_t GetInteger(const char *Name) { return 0; }
	float GetFloat(const char *Name) { return 0.0f; }
	bool Reset(const char *Name) { return true; }
	bool Subscribe(const char *Name, const char *Receiver, FUNC_TYPE Func) { return true; }
	bool UnSubscribe(const char *Name, const char *Receiver) { return true; }
	bool Load(const char *FileName) { return true; }
	bool Save(const char *FileName) { return true; }
	void Dump(void) { }
};

ConfigNull ConfigNullInst;

ConfigIF::ConfigIF() {}
ConfigIF::~ConfigIF() {}

ConfigIF *GetInstance(const char *Name)
{
	ConfigIF *cif = Inst.GetInstance(Name);
	if(cif == NULL)
		return &ConfigNullInst;
	else
		return cif;
}

ConfigIF *Create(const char *Name)
{
	ConfigIF *cif = Inst.Create(Name);
	if(cif == NULL)
		return &ConfigNullInst;
	else
		return cif;
}

bool Destroy(const char *Name)
{
	return Inst.Destroy(Name);
}

} // Config
} // Utils
