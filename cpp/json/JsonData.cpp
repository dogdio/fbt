#include <iostream>
#include <sstream>
#include "JsonData.h"

void JsonData::Set(VARIANT vv)
{
	VarValue = vv;
}

int JsonData::GetInt()
{
	if(VarValue.index() == 0)
		return std::get<int>(VarValue);
	else
		return 0;
}

double JsonData::GetDouble()
{
	if(VarValue.index() == 1)
		return std::get<double>(VarValue);
	else
		return 0.0;
}

bool JsonData::GetBool()
{
	if(VarValue.index() == 2)
		return std::get<bool>(VarValue);
	else
		return false;
}

std::string JsonData::GetString()
{
	if(VarValue.index() == 3)
		return std::get<std::string>(VarValue);
	else
		return "";
}

std::string JsonData::ToString()
{
	std::stringstream ss;

	if(data.size() != 0)
		ss << "{}";
	else if(vdata.size() != 0)
		ss << "[]";
	else if(VarValue.index() == 0)
		ss << std::get<int>(VarValue);
	else if(VarValue.index() == 1)
		ss << std::get<double>(VarValue);
	else if(VarValue.index() == 2)
		ss << std::boolalpha << std::get<bool>(VarValue);
	else if(VarValue.index() == 3)
		ss << '"' << std::get<std::string>(VarValue) << '"';
	else
		ss << "??";

	return ss.str();
}

JSON_MAP &JsonData::Map(void)
{
	return data;
}

JsonData &JsonData::operator[](const std::string &key)
{
	return data[key];
}

std::vector<JsonData> &JsonData::Array(void)
{
	return vdata;
}

JsonData &JsonData::operator[](const size_t index)
{
	if(vdata.size() == 0 || vdata.size() <= index)
		vdata.resize(index+1);

	return vdata[index];
}

namespace {

void TraverseArray(std::vector<JsonData> &vdata, std::string &sp, int depth)
{
	int num = 0;
	for(auto &v : vdata) {
		if(num > 0)
			std::cout << "," << std::endl;

		if(v.Map().size() > 0) {
			std::cout << sp << "  " << "{" << std::endl;
			JsonData::Traverse(v.Map(), depth+2);
			std::cout << "\n" << sp << "  }";
		}
		else
			std::cout << sp << "  " << v.ToString();
		num++;
	}
}

};

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
		else if(m.second.data.size() > 0) {
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

