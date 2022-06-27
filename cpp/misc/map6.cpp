#include <stdio.h>
#include <string>
#include <map>
#include <vector>

// mapのvalueが構造体になるパターン

using MY_VEC = std::vector<int>;
struct Hoge {
	int num1;
	int num2;
	MY_VEC vec;
};
using MY_MAP = std::map<std::string, Hoge>;

MY_MAP Map = {
	{"GO ", {55555, 55, {500, 501} } },
	{"YON", {4444, 44 } },
};

int main(int argc, char **argv)
{
	Map["SAN"] = { 33333, 33 };
	Map["SAN"].vec.push_back(300);
	Map["SAN"].vec.push_back(301);
	Map["SAN"].vec.push_back(302);

	printf("   key     value(struct)\n");
	for(auto it: Map) {
		printf("## %s => num1=%5d, num2=%5d\n", it.first.c_str(), it.second.num1, it.second.num2);
		int i = 0;
		for(auto v: it.second.vec) {
			printf("          vec[%d] = %d\n", i++, v);
		}
	}

	return 0;
}

