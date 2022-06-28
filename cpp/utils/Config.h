#ifndef UTILS_CONFIG_H
#define UTILS_CONFIG_H

#include <stdint.h>

namespace Utils {
namespace Config {

	class ConfigIF {
	public:
		ConfigIF();
		virtual ~ConfigIF();
	
		virtual bool Define(const char *Name, int32_t init, int32_t min, int32_t max) = 0;
		virtual bool Define(const char *Name, float init, float min, float max) = 0;
		virtual bool Define(const char *Name, const char *init, const char *regex, int32_t length) = 0;

		virtual bool Set(const char *Name, const char *Value) = 0;
		virtual bool Set(const char *Name, int32_t Value) = 0;
		virtual bool Set(const char *Name, float Value) = 0;
		virtual const char *GetString(const char *Name) = 0;
		virtual int32_t GetInteger(const char *Name) = 0;
		virtual float GetFloat(const char *Name) = 0;

		// Name==NULL: all values reset(set initial value)
		virtual bool Reset(const char *Name=NULL) = 0;

		// simple setting file:
		//   Name1 = 100
		//   Name2 = 123.45
		//   Name3 = Values
		virtual bool Load(const char *FileName) = 0;
		virtual bool Save(const char *FileName) = 0;
		virtual void Dump(void) = 0;
	};

	ConfigIF *GetInstance(const char *Name);
	ConfigIF *Create(const char *Name);
	bool Destroy(const char *Name);
}
}

#endif
