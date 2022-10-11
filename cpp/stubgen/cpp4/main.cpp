#include <stdio.h>
#include "Hoge.h"

std::map<std::string, int> map1;
std::map<std::string, int> map2;

int main(void)
{
	nsHoge::Hoge h;
	std::map<std::string, int> map3;

	map1["abc"] = 1000;
	map1["def"] = 2000;
	map1["ghi"] = 3000;

	map2["aaa"] = 111;
	map2["bbb"] = 222;
	map2["ccc"] = 333;

	h.Add(10, map1);
	h.Add(20, map2);

	map3 = h.GetMap(20);
	for(auto it: map3)
		printf("## %s => %d\n", it.first.c_str(), it.second);

	map3 = h.GetMap(10);
	for(auto it: map3)
		printf("## %s => %d\n", it.first.c_str(), it.second);

	printf("%d\n", nsHoge::nsHoge2::g_strcmp("123", nsHoge::Str));
	printf("%d\n", g_strcmp2("Hoge", nsHoge::Str));
	return 0;
}
