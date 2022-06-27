#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include <stdint.h>

namespace test {
namespace config {

	class ConfigIF {
	public:
		ConfigIF() {}
		virtual ~ConfigIF() {}
	
		virtual void Set(const char *Name, const char *Value) = 0;
		virtual void Set(const char *Name, uint32_t Value) = 0;
		virtual const char *GetString(const char *Name) = 0;
		virtual uint32_t GetInteger(const char *Name) = 0;
		virtual void Dump(void) = 0;
	};

	ConfigIF *GetInstance(const char *Name);
}
}

#define CONFIG_CATEGORY_GLOBAL "Global"
#define CONFIG_CATEGORY_FUNC1  "Function1"
#define CONFIG_CATEGORY_FUNC2  "Function2"

#endif
