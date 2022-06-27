#include <stdio.h>
#include <map>

using MY_MAP = std::map<int, int>;
MY_MAP Map = {
	{ 1000 , 100 },
	{ 2000 , 200 },
	{ 3000 , 300 },
};
void Dump(void)
{
	for(MY_MAP::iterator it = Map.begin(); it != Map.end(); ++it)
		printf("   %d => %d\n", it->first, it->second);
}

int main(int argc, char **argv)
{
	Dump();

	// ++11 syntax
	// 範囲for文のイテレータを更新しても、元のmapは更新されない
	// (loop中はコピーコンストラクタでauto変数に渡される)
	for(auto it: Map) {
		it.second++; // update(copy領域)
		printf("## %d => %d\n", it.first, it.second);
	}
	Dump();
	printf("\n");

	// 範囲for文ではない、通常のイテレータを更新すると、元のmapが更新される
	// (元のmapを参照しているため)
	for(MY_MAP::iterator it = Map.begin(); it != Map.end(); ++it) {
		it->second++; // update(参照)
		printf("++ %d => %d\n", it->first, it->second);
	}
	Dump();

	return 0;
}
