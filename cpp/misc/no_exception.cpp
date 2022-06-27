#include <stdio.h>
#include <map>

using MY_MAP = std::map<int, int>;
MY_MAP Map = {
	{ 3000 , 333 },
	{ 5000 , 555 },
	{ 4000 , 444 },
};

// -fno-exceptions をつけると try ~~ catch は記述できない
// が、Mapに存在しないkeyを入力すると例外が発生する....
// (ビルドしたコードで例外が発生しないわけではない)
void Find(int key)
{
//	try {
		if(Map.at(key))
			printf("found: %d => %d\n", key, Map[key]);
//	}
//	catch(std::exception &e) {
//		printf("not found: %d \n", key);
//	}
}

int main(int argc, char **argv)
{
	for(auto it: Map)
		printf("## %d => %d\n", it.first, it.second);

	Find(4000); // found

	Find(7777); // not found (out of range)

	printf("can not reach here\n");
	return 0;
}
