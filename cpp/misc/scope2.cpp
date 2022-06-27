#include <stdio.h>

class Hoge3 {
public:
	Hoge3() { printf("Hoge3 INIT:\n"); }
	~Hoge3() { printf("Hoge3 END:\n"); }
};

class Hoge2 {
public:
	Hoge2() { printf(" Hoge2 INIT:\n"); }
	~Hoge2() { printf(" Hoge2 END:\n"); }
	Hoge3 h3; // メンバークラス変数
};

class Hoge {
public:
	Hoge(const char *s) { printf("  Hoge INIT: %s\n", s); s2 = s;}
	~Hoge() { printf("  Hoge END : %s\n", s2); }
	const char *s2;
	Hoge2 h2; // メンバークラス変数
};

int main(void)
{
	// メンバークラス変数(Hoge3,Hoge2)のコンストラクタが先に走る
	// その後、Hogeのコンストラクタが走る
	Hoge h("local scope");

	return 0;
}
// デストラクタはコンストラクタの逆順となる
// Hoge, Hoge2, Hoge3 の順に走る(メンバ変数が後)

