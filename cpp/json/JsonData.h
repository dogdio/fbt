#ifndef JSON_DATA_H
#define JSON_DATA_H

#include <vector>
#include <map>
#include <sstream>

class JsonData;
using JSON_MAP = std::map<std::string, JsonData>;

class JsonData {
public:
	void SetInt(int num);
	int GetInt();
	void SetFloat(float num);
	float GetFloat();
	void SetBool(bool v);
	bool GetBool();
	void SetString(std::string str);
	std::string &GetString();

	JSON_MAP *Map(void);
	JsonData &operator[](const std::string &key);
	JsonData &operator[](const size_t index);

	static void Traverse(JSON_MAP &json_map, int depth = 0);

private:
	std::string ToString();
	std::vector<JsonData> &Array(void);
	static void TraverseArray(std::vector<JsonData> &vdata, std::string &sp, int depth = 0);

	typedef enum {
		VALUE_INT,
		VALUE_FLOAT,
		VALUE_BOOL,
		VALUE_STR,
		VALUE_MAX,
	} VALUE_TYPE;
	VALUE_TYPE ValueType = VALUE_MAX;

	union {
		int IntValue;
		float FloatValue;
		bool BoolValue;
	};
	std::string StringValue;

	JSON_MAP data;
	std::vector<JsonData> vdata;
};

#endif
