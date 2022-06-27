#include <stdio.h>
#include <unordered_map>

using MY_MAP = std::unordered_map<int, int>;
MY_MAP Map = {
	{ 5000 , 555 },
	{ 4000 , 444 },
};
void Find(int key)
{
	try {
		if(Map.at(key))
			printf("found: %d => %d\n", key, Map[key]);
	}
	catch(std::exception &e) {
		printf("not found: %d \n", key);
	}
}

int main(int argc, char **argv)
{
	Map[2000] = 222;
	Map[3000] = 333;
	Map[1000] = 111;
	// unordered_mapではkeyでソートされない

	// iterator: classical syntax
	for(MY_MAP::iterator it = Map.begin(); it != Map.end(); ++it)
		printf("$$ %d => %d\n", it->first, it->second);

	// iterator: ++11 auto variable
	for(auto it: Map)
		printf("## %d => %d\n", it.first, it.second);

	Find(3000);
	Find(9000);

	Map.erase(1000); // key を引数とする
	Map.clear();

	for(auto it: Map) // クリア済みなので何も表示されない
		printf(":: %d => %d\n", it.first, it.second);

	return 0;
}
