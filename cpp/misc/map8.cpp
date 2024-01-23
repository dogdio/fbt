#include <iostream>
#include <map>
#include <string>
// 再帰的 Map と添え字オーバーライド

struct RecursiveMap {
	int num;
    std::map<std::string, RecursiveMap> data;
};
void Dump(void)
{
	// 再帰的にアクセスはできるが、dataメンバを挟まないとダメ
    RecursiveMap result = {};
	result.num = 10;
    result.data["key1"] = {};
    result.data["key1"].num = 100;
    result.data["key1"].data["key2"] = {};
    result.data["key1"].data["key2"].num = 1000;
}

class RecursiveData;
using MY_MAP = std::map<std::string, RecursiveData>;

class RecursiveData {
public:
	// 添え字をオーバーライド
	RecursiveData &operator[](const std::string &key) {
		return data[key]; // keyを新規作成 or 上書き
	}
	void SetNum(int num) {
		Num = num;
	}
	int GetNum() {
		return Num;;
	}
	static void Traverse(MY_MAP &mymap, int depth = 0) {
		std::string sp;
		for(int i = 0; i < depth; i++)
			sp += "  ";
		for(auto &m : mymap) {
			std::cout << sp << m.first << "=>" << m.second.GetNum() << std::endl;
			Traverse(m.second.data, depth+1);
		}
	}
private:
	int Num;
	MY_MAP data;
};

void Dump2(void)
{
	MY_MAP Map = {};
	Map["key1"].SetNum(1);
	Map["key10"].SetNum(10);

	// 添え字から添え字にアクセスできる(何段でもOK)
	Map["key1"]["key2"].SetNum(22);
	Map["key1"]["key3"]["key4"].SetNum(444); // key3は初期値のまま
	Map["key1"]["key5"].SetNum(55);

	RecursiveData::Traverse(Map);
}

int main(void)
{
	Dump();
	Dump2();
}
