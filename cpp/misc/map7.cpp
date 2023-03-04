#include <stdio.h>
#include <string>
#include <map>
#include <vector>
// mapのvalueがclass(実体) になるパターン with const map

// コンストラクタの引数あり、なしの両方を定義する
class Hoge {
public:
	Hoge() {
//		printf("INIT1 \n");
	}
	Hoge(int n1, int n2) : num1(n1), num2(n2) {
		printf("INIT2 %d, %d\n", num1, num2);
	}
	~Hoge() {
//		printf("EXIT %d, %d\n", num1, num2);
	}
	Hoge(const Hoge& rv) { // Copy Constructor
		printf(" COPY \n");
		num1 = rv.num1;
		num2 = rv.num2;
	}
	int num1;
	int num2;
};

using MY_MAP = std::map<std::string, Hoge>;
MY_MAP Map = {
	{"GO", Hoge(55555, 55) }, // INIT2 + COPY,COPY
	{"YON", Hoge(4444, 44) }, // INIT2 + COPY,COPY
};

const MY_MAP &GetMap1(void) { return Map; } // const ref
      MY_MAP &GetMap2(void) { return Map; } // ref
      MY_MAP  GetMap3(void) { return Map; } // value

int main(int argc, char **argv)
{
	printf("<<<<<<<<< main start >>>>>>>> \n");
	const MY_MAP &m = GetMap1();

	//m.at("GO").num1 = 1000; // build error. constは書き込みできない

	// constだと添字はNG (未定義のkeyだと、自動で書き込みが行われるため)
	// -fpermissive で error を warning に変更可能
	//int num0 = m["GO"].num1;  // build error

	int num1 = m.at("GO").num1; // at()はOK

	try {
		int fail = m.at("FAIL").num1; // 未定義のkeyは例外発生
	}
	catch (std::exception &e) {
		printf("not found\n");
	}

	printf("not copy: num1=%d\n\n", GetMap2()["YON"].num1);

	printf("copy x2 : num1=%d\n\n", GetMap3()["YON"].num1);

	printf("###\n");
	GetMap3(); // コピーコンストラクタ x2
	printf("### only call \n\n");

	MY_MAP m2 = GetMap1(); // 参照を実体へコピー(コピーコンストラクタ x2)
	printf("copy x2 : num1=%d\n\n", m2["GO"].num1);

	Hoge h = m2["YON"]; // Hogeだけをコピーする場合: COPY x1
	return 0;
}

