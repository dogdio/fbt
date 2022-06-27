#include <stdio.h>
// コンストラクタでのメンバ初期化 with 継承
// ChildとParentでコンストラクタの引数を合わせておく
// Parentでの引数の定義順は初期化順と同じにする
class Parent1 {
public:
	Parent1(int v, int x) : 
			pub_value(v), pub_value2(x) { }
#if 0 // WARNING: *** will be initialized after
	int pub_value2;
	int pub_value;
#else
	int pub_value;
	int pub_value2;
#endif
};

class Child1 : public Parent1{
public:
	Child1(int a, int b);
};

// Childのコンストラクタ実装(:以降を書く)
//Child1::Child1(int a, int b) // compile error
Child1::Child1(int a, int b) : Parent1(a, b)
{

}

int main(void)
{
	Child1 c1(10, 20);
	printf("v=%d, v2=%d\n", c1.pub_value, c1.pub_value2);
	return 0;
}
