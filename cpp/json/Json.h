#ifndef JSON_H
#define JSON_H

#include "JsonData.h"

class Json {
public:
	bool ParseFile(std::string file);
	bool Parse(std::string &str);
	void Dump(void);
	bool Save(std::string file);
	JSON_MAP &GetRoot(void);

private:
	bool ParseObjectStart(const char **buff);
	bool ParseObject(const char **buff);
	bool ParseObjectEnd(const char **buff);

	bool ParseObjects(const char **buff, bool *IsLast, int index);
	bool ParseArrayEnd(const char **buff);

	bool ParseValue(const char **buff, bool *IsLast, bool IsArray=false, int index=0);
	bool ParseValueAsObject(const char **buff);
	bool ParseValueAsArray(const char **buff);

	bool ParseKey(const char **buff);
	bool ParseString(const char **buff, char *str);
	bool ParseNumber(const char **buff, bool IsArray, int index);

	const static int BUFF_SIZE = 256;
	char key[BUFF_SIZE] = {};
	char value[BUFF_SIZE] = {};
	int idx;
	JSON_MAP Map = {};
	JSON_MAP *Curr = &Map;
};

#endif
