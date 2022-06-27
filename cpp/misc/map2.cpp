#include <stdio.h>
#include <vector>
#include <map>

// mapのvalueがvectorになるパターン
using MY_VEC = std::vector<int>;
using MY_MAP = std::map<int, MY_VEC>;
MY_MAP Map = {
	{ 5000 , { 555, 55, 5 } },
	{ 4000 , { 4, 44, 444, 4444 } },
};

int main(int argc, char **argv)
{
	Map[3000].push_back(3333);
	Map[3000].push_back(333);

	printf("   key    value(vector)\n");

	for(auto it: Map) {
		int i = 0;
		for(auto v: it.second) {
			printf("## %d => [%d] %d\n", it.first, i++, v);
		}
	}
	return 0;
}

