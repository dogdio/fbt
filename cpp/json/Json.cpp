#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"
#include "Json.h"

using namespace Utils;

namespace {

class JsonPriv : public Json::JsonIF {
public:
	JsonPriv(const char *Name)
	{
		if(Name == NULL)
			throw std::invalid_argument("Name is null");
		MyName = Name;
	} 
	~JsonPriv() {}

	bool ParseFile(std::string file) override;
	bool Parse(std::string &str) override;
	void Dump(void) override;
	bool Save(std::string file) override;
	JSON_MAP &GetRoot(void) override;

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
	void SetValue(bool IsArray, int index, VARIANT vv);

	const static int BUFF_SIZE = 256;
	char key[BUFF_SIZE] = {};
	char value[BUFF_SIZE] = {};
	int idx;
	JSON_MAP Map = {};
	JSON_MAP *Curr = &Map;
	const char *MyName;
};

Factory::FactoryIF<JsonPriv, Json::JsonIF> Inst("Utils.Json");

bool IsBool(char *str, bool &bv)
{
	if(strcmp(str, "true") == 0) {
		bv = true;
		return true;
	}
	else if(strcmp(str, "false") == 0) {
		bv = false;
		return true;
	}
	else
		return false;
}

bool IsDouble(char *str, double &dv)
{
	char *endptr;

	dv = strtof(str, &endptr);
	if(*endptr == '\0')
		return true;
	else
		return false;
}

bool IsInteger(char *str, int &iv)
{
	char *endptr;

	iv = strtol(str, &endptr, 10);
	if(*endptr == '\0')
		return true;
	else
		return false;
}

};

void JsonPriv::SetValue(bool IsArray, int index, VARIANT vv)
{
	if(IsArray)
		(*Curr)[key][index].Set(vv);
	else
		(*Curr)[key].Set(vv);
}

bool JsonPriv::ParseNumber(const char **buff, bool IsArray, int index)
{
	const char *p = *buff;
	bool bv;
	int iv;
	double dv;
	idx = 0;

	while (*p != '\0') {
//		std::cout << "!! " << *p <<std::endl;
		if(*p == '+' || *p == '-' || *p == '.' || (0x30 <= *p && *p <= 0x39) ||
			(0x61 <= *p && *p <= 0x75)) { // a~u
			value[idx++] = *p;
			if(idx == BUFF_SIZE)
				return false;
		}
		else {
			value[idx] = '\0';
			if(IsInteger(value, iv))
				SetValue(IsArray, index, iv);
			else if(IsDouble(value, dv))
				SetValue(IsArray, index, dv);
			else if(IsBool(value, bv))
				SetValue(IsArray, index, bv);
			else
				return false;

			p += strspn(p, " \t\r\n");
			*buff = p;
			return true;
		}
		p++;
	}
	return false;
}

bool JsonPriv::ParseString(const char **buff, char *str)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	if(*p != '"')
		return false;

	p++;
	idx = 0;
	do {
		if(*p == '"') {
			str[idx] = '\0';
			*buff = p + 1;
			return true;
		}
		else {
			str[idx++] = *p;
			if(idx == BUFF_SIZE)
				return false;
		}
		p++;
	} while(*p != '\0');

	return false;
}

bool JsonPriv::ParseObjects(const char **buff, bool *IsLast, int index)
{
	if(!ParseObjectStart(buff)) // '{'
		return false;
	
	JSON_MAP *Save = Curr;
	std::string key_save = key;
	Curr = &(*Curr)[key][index].Map();

	bool IsLast2 = false;
	while(!IsLast2) {
		if(!ParseKey(buff))
			return false;
	
		if(!ParseValue(buff, &IsLast2))
			return false;
	}
	Curr = Save;
	strcpy(key, key_save.c_str());

	if(!ParseObjectEnd(buff)) // '}'
		return false;

	const char *p = *buff;
	p += strspn(p, " \t\r\n");
	if(*p == ',')
		*buff = p+1;
	else
		*IsLast = true;

	return true;
}

bool JsonPriv::ParseValueAsArray(const char **buff)
{
	bool IsObjects = false;
	const char *p = *buff;
	p += strspn(p, " \t\r\n");
	if(*p == '{')
		IsObjects = true;

	int index2 = 0;
	bool IsLast2 = false;
	while(!IsLast2) {
		if(IsObjects) { // "key" : [ { ... }, { ... } ]
			if(!ParseObjects(buff, &IsLast2, index2++))
				return false;
		}
		else { // "key" : [1, 2, 3]
			if(!ParseValue(buff, &IsLast2, true, index2++))
				return false;
		}
//		std::cout << "===" << *buff << "==="<< std::endl;
	}
	if(!ParseArrayEnd(buff))
		return false;

	return true;
}

bool JsonPriv::ParseValueAsObject(const char **buff)
{
	JSON_MAP *Save = Curr;
	Curr = &(*Curr)[key].Map();

	if(!ParseObject(buff))
		return false;

	if(!ParseObjectEnd(buff))
		return false;

	Curr = Save;
	return true;
}

bool JsonPriv::ParseValue(const char **buff, bool *IsLast, bool IsArray, int index)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	if(*p == '[') { // "key" : [1, 2, 3]
		*buff = p+1;
		if(!ParseValueAsArray(buff))
			return false;
	}
	else if(*p == '{') { // "key" : { "aaa": 123 }
		*buff = p+1;
		if(!ParseValueAsObject(buff))
			return false;
	}
	else if(*p == '"') { // "key" : "value"
		if(!ParseString(buff, value))
			return false;
		SetValue(IsArray, index, value);
	}
	else { // "key" : 12345 or true or false or -12.345
		*buff = p;
		if(!ParseNumber(buff, IsArray, index))
			return false;
	}

	p = *buff;
	p += strspn(p, " \t\r\n");
	if(*p == '}' || *p == ']') {
		*IsLast = true;
		*buff = p;
	}
	else if(*p == ',') {
		*buff = p+1;
	}
	else {
		return false;
	}
	return true;
}

bool JsonPriv::ParseKey(const char **buff)
{
	if(!ParseString(buff, key))
		return false;

	const char *p = *buff;
	p += strspn(p, " \t\r\n");
	if(*p != ':')
		return false;

//	std::cout << key << " = ";
	*buff = p + 1;
	return true;
}

bool JsonPriv::ParseObject(const char **buff)
{
	bool IsLast = false;

	// {
	//   "key1" : 12,
	//   "key2" : 34,
	//   "key3" : 56 <---- IsLast = true
	// }
	while(!IsLast) {
		if(!ParseKey(buff)) // "key1" :
			return false;
	
		if(!ParseValue(buff, &IsLast)) // 12,
			return false;
	}
	return true;
}

bool JsonPriv::ParseObjectStart(const char **buff)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	*buff = p + 1;

	if(*p == '{')
		return true;
	else
		return false;
}

bool JsonPriv::ParseObjectEnd(const char **buff)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	*buff = p + 1;

	if(*p == '}')
		return true;
	else
		return false;
}

bool JsonPriv::ParseArrayEnd(const char **buff)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	*buff = p + 1;

	if(*p == ']')
		return true;
	else
		return false;
}

bool JsonPriv::Parse(std::string &str)
{
	const char *buff = str.c_str();

	if(!ParseObjectStart(&buff))
		return false;
	
	if(!ParseObject(&buff))
		return false;
	
	if(!ParseObjectEnd(&buff))
		return false;

	return true;
}

bool JsonPriv::ParseFile(std::string file)
{
	std::ifstream ifs(file);
	std::string json_string = "";

	if(!ifs.is_open())
		return false;

	std::string line;
	while(std::getline(ifs, line)) {
		json_string += line;
		json_string += "\n";
	}
	ifs.close();

	return Parse(json_string);
}

void JsonPriv::Dump(void)
{
	std::cout << "{" << std::endl;
	JsonData::Traverse(Map);
	std::cout << "\n}" << std::endl;
}

bool JsonPriv::Save(std::string file)
{
	std::streambuf *save = std::cout.rdbuf();
	std::ofstream ofs(file);

	if(!ofs.is_open())
		return false;

	std::cout.rdbuf(ofs.rdbuf());
	Dump();
	std::cout.rdbuf(save);

	ofs.close();
	return true;
}

JSON_MAP &JsonPriv::GetRoot(void)
{
	return Map;
}


namespace Json {

class JsonNull : public Json::JsonIF {
public:
	bool ParseFile(std::string file) { return true; }
	bool Parse(std::string &str) { return true; }
	void Dump(void) { }
	bool Save(std::string file) { return true; }
	JSON_MAP &GetRoot(void) { return data; }
	JSON_MAP data;
};
JsonNull JsonNullInst;

JsonIF::JsonIF() {}
JsonIF::~JsonIF() {}

JsonIF *GetInstance(const char *Name)
{
	JsonIF *jif = Inst.GetInstance(Name);
	if(jif == NULL)
		return &JsonNullInst;
	else
		return jif;
}

JsonIF *Create(const char *Name)
{
	JsonIF *jif = Inst.Create(Name);
	if(jif == NULL)
		return &JsonNullInst;
	else
		return jif;
}

bool Destroy(const char *Name)
{
	return Inst.Destroy(Name);
}

}

