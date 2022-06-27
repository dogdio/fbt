#include <stdio.h>
#include <string>
#include <map>
// mapのvalueがclass(実体) になるパターン

// コンストラクタの引数あり、なしの両方を定義する
class Hoge {
public:
	Hoge() {
//		printf("INIT1 \n");
	}
	Hoge(int n1, int n2) : num1(n1), num2(n2) {
//		printf("INIT2 %d, %d\n", num1, num2);
	}
	~Hoge() {
//		printf("EXIT %d, %d\n", num1, num2);
	}
	int num1;
	int num2;
};

using MY_MAP = std::map<std::string, Hoge>;
MY_MAP Map = {
	{"GO ", Hoge(55555, 55) },
	{"YON", Hoge(4444, 44) },
};
// クラス(Hoge)変数を生成しMapに代入している
// コンストラクタ、コピーコンストラクタx2 がmain()の前に走る

int main(int argc, char **argv)
{
	Map["SAN"] = Hoge(333, 33);

	printf("   key     value(class)\n");
	for(auto it: Map) {
		printf("## %s => { num1=%5d, num2=%5d }\n", it.first.c_str(), it.second.num1, it.second.num2);
	}

	return 0;
}

