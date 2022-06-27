#include <stdio.h>

class Hoge {
public:
	Hoge(const char *s) { printf("INIT: %s\n", s); s2 = s;}
	~Hoge() { printf("END : %s\n", s2); }
	const char *s2;
};

// main()の前にグローバル領域のコンストラクタが走る
Hoge h("global scope");

int main(void)
{
	printf(">>>>> main start\n");
	Hoge h2("  local scope");

	// このスコープ内でh3のコンストラクタ、デストラクタが走る
	{
		Hoge h3("    brackets");
	}

	printf("<<<<< main end\n");
	return 0;
	// h2 のデストラクタはmain()を抜けるときに走る
}
// main()が完全に終わってから、h のデストラクタが走る

