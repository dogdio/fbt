#include <stdio.h>
#include <string>
#include <unordered_map>
#include "TestConfig.h"
#include "TestConfigName.h"

using namespace test;

namespace {

using CONFIG_STR = std::unordered_map<std::string, const char *>;
using CONFIG_INT = std::unordered_map<std::string, uint32_t>;
using CONFIG_MAP = std::unordered_map<std::string, config::ConfigIF *>;

class TestConfig : public config::ConfigIF {

public:
	TestConfig(const char *Name);
	~TestConfig();

	void Set(const char *Name, const char *Value) override;
	void Set(const char *Name, uint32_t Value) override;
	const char *GetString(const char *Name) override;
	uint32_t GetInteger(const char *Name) override;
	void Dump(void) override;

	CONFIG_STR ConfigStr;
	CONFIG_INT ConfigInt;
};

// instantiation myself.
TestConfig Instance1(CONFIG_CATEGORY_GLOBAL);
TestConfig Instance2(CONFIG_CATEGORY_FUNC1);
TestConfig Instance3(CONFIG_CATEGORY_FUNC2);

CONFIG_MAP &GetConfigMap(void)
{
	// local static: generate instance when this function called.
	static CONFIG_MAP ConfigMap;
	return ConfigMap;
}

void SetConfigInstance(const char *Name, config::ConfigIF *arg)
{
	CONFIG_MAP &Map = GetConfigMap();
	Map[Name] = arg;
}

} // anonymous

///////////////////////////////////////////////////////////
//
// TestConfig Class
//
///////////////////////////////////////////////////////////
TestConfig::TestConfig(const char *Name)
{
	SetConfigInstance(Name, this); // push to Factory automatically
}

TestConfig::~TestConfig()
{

}
void TestConfig::Set(const char *Name, const char *Value)
{
	ConfigStr[Name] = Value;
}

void TestConfig::Set(const char *Name, uint32_t Value)
{
	ConfigInt[Name] = Value;
}

const char *TestConfig::GetString(const char *Name)
{
	if(ConfigStr[Name] == NULL)
		return "";
	else
		return ConfigStr[Name];
}

uint32_t TestConfig::GetInteger(const char *Name)
{
	return ConfigInt[Name];
}

void TestConfig::Dump(void)
{
	for(auto it : ConfigStr)
		printf("S: '%s' = %s\n", it.first.c_str(), it.second);
	for(auto it : ConfigInt)
		printf("I: '%s' = %d\n", it.first.c_str(), it.second);
}

namespace test {
namespace config {

ConfigIF *GetInstance(const char *Name)
{
	CONFIG_MAP &Map = GetConfigMap();
	try {
		return Map[Name];
	}
	catch (const std::exception & e){
		printf("not found: %s\n", Name);
		return NULL;
	}
}

}
}
