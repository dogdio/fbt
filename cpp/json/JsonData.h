#ifndef JSON_DATA_H
#define JSON_DATA_H

#include <vector>
#include <map>
#include <variant>

class JsonData;
using JSON_MAP = std::map<std::string, JsonData>;
using VARIANT = std::variant<int, double, bool, std::string>;

class JsonData {
public:
	// root["key"].Set()
	void Set(VARIANT vv);
	int GetInt();
	double GetDouble();
	bool GetBool();
	std::string GetString();

	// for(auto &a : root["key"].Map()) { ... }
	JSON_MAP &Map(void);

	// root["key"]["xxx"].Set()
	JsonData &operator[](const std::string &key);

	// for(auto &a : root["key"].Array()) { ... }
	std::vector<JsonData> &Array(void);

	// root["key"][0].Set()
	JsonData &operator[](const size_t index);

	std::string ToString();
	static void Traverse(JSON_MAP &json_map, int depth = 0);

private:
	VARIANT VarValue;
	JSON_MAP data;
	std::vector<JsonData> vdata;
};

#endif
