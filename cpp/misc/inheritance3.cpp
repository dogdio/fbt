#include <stdio.h>

class Parent1 {
public:
	virtual void Func1(void) { printf("Parent1 Func1\n"); }
};
class Parent2 {
public:
	void Func2(void) { printf("Parent2 Func2\n"); }
};

// 他重継承(一つの子供に対して、親が複数ある)
//   Parent1, Parent2 <-- Child1
class Child1 : public Parent1, public Parent2 {
public:
	void Func1(void) { printf("Child1 Func1\n"); }
	void Func2(void) { printf("Child1 Func2\n"); }
};

int main(void)
{
	// Childクラスをどちらの親にも代入可能
	Parent1 *p1 = new Child1();
	Parent2 *p2 = new Child1();
	Child1  *c1 = new Child1();

	// Childでオーバーライドしているので、Childの関数が呼ばれる
	p1->Func1();
//	p1->Func2(); // compile error

	// 同名関数がChild,Parentにあるがvirtualではない場合
	// そのインスタンス(p2)の関数が呼ばれる
	p2->Func2();
//	p2->Func1(); // compile error

	// Childインスタンスなので、Childの関数が呼ばれる
	c1->Func1();
	c1->Func2();

	return 0;
}

