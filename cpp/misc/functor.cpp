#include <stdio.h>
// ファンクタ
// 構造体の中でoperatorをオーバーライド
//
// データメンバと関数をセットで管理できるが、、
// 構造体を別のモジュールに渡してコールバック関数のように使うことはできるかも

struct Hoge {
	int operator()(int Num)
	{
		if(v % 2 == 0) {
			v2 += Num*2;
			return Num*100;
		}
		else {
			v2 += Num*3;
			return Num*10;
		}
	}
	int v;
	int v2;
};

int main(void)
{
	int ret;
#if 1 // 以下は同じ
	Hoge h = {.v=1, .v2=10};
#else
	Hoge h = { };
	h.v = 1;
	h.v2 = 10;
#endif

	ret = h(100);
	printf("ret=%d, v2=%d\n", ret, h.v2);
	return 0;
}

