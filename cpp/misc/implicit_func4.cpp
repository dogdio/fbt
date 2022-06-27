#include <stdio.h>
#include <utility>

class Hoge {
public:
	Hoge(int n) {
		Num = n;
		printf("Constructor %p: %d\n", this, Num);
	}
	~Hoge() {
		printf("Destructor %p: %d\n", this, Num);
	}
	// ++11 syntax
	Hoge(const Hoge& rv) = default;            // Copy Constructor
	Hoge& operator=(const Hoge& rv) = default; // Copy Assignment Operator
	Hoge(Hoge&& rv) = default;                 // Move Constructor
	Hoge& operator=(Hoge&& rv) = default;      // Move Assignment Operator
	int Num;
};

// コピーコンストラクタ, コピー代入演算子を許可(default動作)
// ムーブコンストラクタ, ムーブ代入演算子を許可(default動作)
int main(void)
{
	Hoge h1(100);
	Hoge *h2 = new Hoge(200);
	printf(" : h1.Num=%d, H2.Num=%d\n\n", h1.Num, h2->Num);

	// invoke Copy Constructor (not Constructor)
	Hoge h3 = h1;
	printf("C: h1.Num=%d, H2.Num=%d, H3.Num=%d\n\n", h1.Num, h2->Num, h3.Num);

	// invoke Copy Assignment Operator (not Constructor)
	h1 = *h2;
	printf("C: h1.Num=%d, H2.Num=%d, H3.Num=%d\n\n", h1.Num, h2->Num, h3.Num);

	// invoke Move Constructor (not Constructor)
	Hoge h4 = std::move(h1);
	printf("M: h1.Num=%d, H2.Num=%d, H4.Num=%d\n\n", h1.Num, h2->Num, h4.Num);

	// invoke Move Assignment Operator (not Constructor)
	h1 = std::move(*h2);
	printf("M: h1.Num=%d, H2.Num=%d, H4.Num=%d\n\n", h1.Num, h2->Num, h4.Num);

	// ムーブコンストラクタ、ムーブ代入演算子のdefault動作は、単にコピーするだけなので、
	// コピー元の値は変化しない
}

