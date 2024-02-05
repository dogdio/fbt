#ifndef JSON_H
#define JSON_H

#include "JsonData.h"

namespace Json {

	class JsonIF {
	public:
		JsonIF();
		virtual ~JsonIF();

		virtual bool ParseFile(std::string file) = 0;
		virtual bool Parse(std::string &str) = 0;
		virtual void Dump(void) = 0;
		virtual bool Save(std::string file) = 0;
		virtual JSON_MAP &GetRoot(void) = 0;
	};

	JsonIF *GetInstance(const char *Name);
	JsonIF *Create(const char *Name);
	bool Destroy(const char *Name);
};

#endif
