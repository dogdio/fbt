#include <iostream>
#include "JsonData.h"

void JsonData::SetInt(int num)
{
	IntValue = num;
	ValueType = VALUE_INT;
}

int JsonData::GetInt()
{
	return IntValue;
}

void JsonData::SetFloat(float num)
{
	FloatValue = num;
	ValueType = VALUE_FLOAT;
}

float JsonData::GetFloat()
{
	return FloatValue;
}

void JsonData::SetBool(bool v)
{
	BoolValue = v;
	ValueType = VALUE_BOOL;
}

bool JsonData::GetBool()
{
	return BoolValue;
}

void JsonData::SetString(std::string str)
{
	StringValue = str;
	ValueType = VALUE_STR;
}

std::string &JsonData::GetString()
{
	return StringValue;
}

std::string JsonData::ToString()
{
	std::stringstream ss;
	if(ValueType == VALUE_INT)
		ss << IntValue;
	else if(ValueType == VALUE_BOOL)
		ss << std::boolalpha << BoolValue;
	else if(ValueType == VALUE_FLOAT)
		ss << FloatValue;
	else if(ValueType == VALUE_STR)
		ss << '"' << StringValue << '"';
	else
		ss << "??";

	return ss.str();
}

std::vector<JsonData> &JsonData::Array(void)
{
	return vdata;
}

JSON_MAP *JsonData::Map(void)
{
	return &data;
}

JsonData &JsonData::operator[](const std::string &key)
{
	return data[key];
}

JsonData &JsonData::operator[](const size_t index)
{
	if(vdata.size() == 0 || vdata.size() <= index)
		vdata.resize(index+1);

	return vdata[index];
}

void JsonData::TraverseArray(std::vector<JsonData> &vdata, std::string &sp, int depth)
{
	int num = 0;
	for(auto &v : vdata) {
		if(num > 0)
			std::cout << "," << std::endl;

		if(v.ValueType == VALUE_MAX) {
			std::cout << sp << "  " << "{" << std::endl;
			Traverse(v.data, depth+2);
			std::cout << "\n" << sp << "  }";
		}
		else
			std::cout << sp << "  " << v.ToString();
		num++;
	}
}

void JsonData::Traverse(JSON_MAP &json_map, int depth)
{
	std::string sp;
	for(int i = 0; i < depth+1; i++)
		sp += "  ";

	int num = 0;
	for(auto &m : json_map) {
		if(num > 0)
			std::cout << "," << std::endl;

		std::cout << sp << '"' << m.first << "\": "; // output "key: "
		if(m.second.vdata.size() > 0) {
			std::cout << "[" << std::endl;
			TraverseArray(m.second.vdata, sp, depth);
			std::cout << "\n" << sp << "]";
		}
		else if(m.second.ValueType == VALUE_MAX) {
			std::cout << "{" << std::endl;
			Traverse(m.second.data, depth+1);
			std::cout << "\n" << sp << "}";
		}
		else {
			std::cout << m.second.ToString();
		}
		num++;
	}
}

