#include <stdio.h>

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
	Hoge(const Hoge& rv) = delete;            // Copy Constructor
	Hoge& operator=(const Hoge& rv) = delete; // Copy Assignment Operator
	Hoge(Hoge&& rv) = delete;                 // Move Constructor
	Hoge& operator=(Hoge&& rv) = delete;      // Move Assignment Operator
	int Num;
};

void HogeRefs(Hoge *h)
{
	printf("h.Num=%d\n", h->Num);
}
void HogeCopy(Hoge h)
{
	printf("h.Num=%d\n", h.Num);
}

// コピーコンストラクタ, コピー代入演算子を禁止
// ムーブコンストラクタ, ムーブ代入演算子を禁止
int main(void)
{
	Hoge h1(100);
	Hoge *h2 = new Hoge(200);
	Hoge *h3 = &h1; // アドレス渡し
	HogeRefs(h3);   // アドレス渡し

#if 0 // compile error
	Hoge h3 = h1; // Copy Constructor
	HogeCopy(h1); // Copy Constructor
	h1 = *h2;     // Copy Assignment Operator
#endif
#if 0 // compile error
	Hoge h3 = std::move(h1); // Move Constructor
	HogeCopy(std::move(h1)); // Move Constructor
	h1 = std::move(*h2);     // Move Assignment Operator
#endif

	delete h2;
}

