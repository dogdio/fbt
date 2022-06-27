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
	Hoge(const Hoge& rv) = delete;            // Copy Constructor
	Hoge& operator=(const Hoge& rv) = delete; // Copy Assignment Operator
	Hoge(Hoge&& rv) { // ムーブコンストラクタ
		printf("Move Constructor: this=%p, rv=%p\n", this, &rv);
		Num = rv.Num;
		rv.Num = 0; // clear copy src
	}
	Hoge& operator=(Hoge&& rv) { // ムーブ代入演算子
		printf("Move Assignment Operator: this=%p, rv=%p\n", this, &rv);
		if(this != &rv) {
			Num = rv.Num;
			rv.Num = 0; // clear copy src
		}
		return *this;
	}
	int Num;
};

// ムーブコンストラクタ, ムーブ代入演算子をオーバーロード
int main(void)
{
	Hoge h1(100);
	Hoge *h2 = new Hoge(200);
	printf("h1.Num=%d, h2.Num=%d\n\n", h1.Num, h2->Num);

	{
		// invoke Move Constructor (not Constructor)
		Hoge h3 = std::move(h1);
		printf("Move: h1 to h3\n");
		printf("h1.Num=%d, h2.Num=%d, h3.Num=%d\n\n", h1.Num, h2->Num, h3.Num);
    
		// invoke Move Assignment Operator (not Constructor)
		h1 = std::move(*h2);
		printf("Move: h2 to h1\n");
		printf("h1.Num=%d, h2.Num=%d, h3.Num=%d\n\n", h1.Num, h2->Num, h3.Num);
	}
	// h3 destructor

	delete h2;
	return 0;
}

