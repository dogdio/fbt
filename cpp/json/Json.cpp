#include <string>
#include <iostream>
#include <cstring>
#include <Json.h>

namespace {

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

bool IsFloat(char *str, float &fv)
{
	char *endptr;

	fv = strtof(str, &endptr);
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

bool Json::ParseNumber(const char **buff, bool IsArray, int index)
{
	const char *p = *buff;
	bool bv;
	int iv;
	float fv;
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
			if(IsInteger(value, iv)) {
//				std::cout << "I:" << key << ", " << value << std::endl;
				if(IsArray)
					(*Curr)[key][index].SetInt(iv);
				else
					(*Curr)[key].SetInt(iv);
			}
			else if(IsFloat(value, fv)) {
//				std::cout << "F:" << key << ", " << fv << std::endl;
				if(IsArray)
					(*Curr)[key][index].SetFloat(fv);
				else
					(*Curr)[key].SetFloat(fv);
			}
			else if(IsBool(value, bv)) {
//				std::cout << "B:" << key << ", " << bv << std::endl;
				if(IsArray)
					(*Curr)[key][index].SetBool(bv);
				else
					(*Curr)[key].SetBool(bv);
			}
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

bool Json::ParseString(const char **buff, char *str)
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

bool Json::ParseObjects(const char **buff, bool *IsLast, int index)
{
	if(!ParseObjectStart(buff)) // '{'
		return false;
	
	JSON_MAP *Save = Curr;
	std::string key_save = key;
	Curr = (*Curr)[key][index].Map();

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

bool Json::ParseValueAsArray(const char **buff)
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

bool Json::ParseValueAsObject(const char **buff)
{
	JSON_MAP *Save = Curr;
	Curr = (*Curr)[key].Map();

	if(!ParseObject(buff))
		return false;

	if(!ParseObjectEnd(buff))
		return false;

	Curr = Save;
	return true;
}

bool Json::ParseValue(const char **buff, bool *IsLast, bool IsArray, int index)
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
//		std::cout << "S:" << key << ", " << value << std::endl;
		if(IsArray)
			(*Curr)[key][index].SetString(value);
		else
			(*Curr)[key].SetString(value);
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

bool Json::ParseKey(const char **buff)
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

bool Json::ParseObject(const char **buff)
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

bool Json::ParseObjectStart(const char **buff)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	*buff = p + 1;

	if(*p == '{')
		return true;
	else
		return false;
}

bool Json::ParseObjectEnd(const char **buff)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	*buff = p + 1;

	if(*p == '}')
		return true;
	else
		return false;
}

bool Json::ParseArrayEnd(const char **buff)
{
	const char *p = *buff;

	p += strspn(p, " \t\r\n");
	*buff = p + 1;

	if(*p == ']')
		return true;
	else
		return false;
}

bool Json::Parse(std::string &str)
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

void Json::Dump(void)
{
	std::cout << "{" << std::endl;
	JsonData::Traverse(Map);
	std::cout << "\n}" << std::endl;
}

JSON_MAP &Json::GetRoot(void)
{
	return Map;
}

