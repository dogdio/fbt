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
	Hoge(const Hoge& rv) { // コピーコンストラクタ
		printf("Copy Constructor: this=%p, rv=%p\n", this, &rv);
		Num = rv.Num; // そのまま
	}
	Hoge& operator=(const Hoge& rv) { // コピー代入演算子
		printf("Copy Assignment Operator: this=%p, rv=%p\n", this, &rv);
		if(this != &rv) {
			Num = rv.Num * 2; // 2倍
		}
		return *this;
	}
	Hoge(Hoge&& rv) = delete;                 // Move Constructor
	Hoge& operator=(Hoge&& rv) = delete;      // Move Assignment Operator
	int Num;
};

// コピーコンストラクタ, コピー代入演算子をオーバーロード
int main(void)
{
	Hoge h1(100);
	Hoge *h2 = new Hoge(200);
	printf("h1.Num=%d, H2.Num=%d\n\n", h1.Num, h2->Num);

	// invoke Copy Constructor (not Constructor)
	Hoge h3 = h1;
	printf("h1.Num=%d, H2.Num=%d, H3.Num=%d\n\n", h1.Num, h2->Num, h3.Num);

	// invoke Copy Assignment Operator (not Constructor)
	h1 = *h2;
	printf("h1.Num=%d, H2.Num=%d, H3.Num=%d\n\n", h1.Num, h2->Num, h3.Num);
	return 0;
}

