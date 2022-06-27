#include <stdio.h>
#include <string>
#include <map>

// mapのvalueがmapになるパターン
using MY_MAP2 = std::map<int, int>;
using MY_MAP = std::map<std::string, MY_MAP2>;
MY_MAP Map = { // カッコが多いので注意
	{ "GO ", { { 5555, 555 }, {500, 50}, {50, 5} } },
	{ "YON", { { 444, 44 }, {400, 40} } },
};

void Find(const char *key, int key2)
{
	try {
		if(Map.at(key).at(key2))
			printf("found: [%s][%d] => %d \n", key, key2, Map[key][key2]);
	}
	catch(std::exception &e) {
		printf("not found: [%s][%d] \n", key, key2);
	}
}

int main(int argc, char **argv)
{
	Map["SAN"][333] = 33;
	Map["SAN"][300] = 30;

	printf("   key     value(map)\n");
	for(auto it: Map) {
		for(auto v: it.second) {
			printf("## %s => { %d => %d }\n", it.first.c_str(), v.first, v.second);
		}
	}

	Find("YON", 444);
	Find("YON", 555);
	Find("NULL", 0);
	return 0;
}

