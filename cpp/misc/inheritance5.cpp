#include <stdio.h>

class Parent1 {
	friend class P1_friend;
public:
	Parent1(int a, int b, int c) :
		pub_value(a), pri_value(b), pro_value(c) { }
	int pub_value;
private:
	int pri_value;
protected:
	int pro_value;
};

class Child1 : public Parent1 {
public:
	Child1(int a, int b, int c) : Parent1(a,b,c) { }

	// Parent1を継承したclassは、public, protected にアクセス可能
	void func(void)
	{
		printf("inheri: c1.pub=%d\n", pub_value);
//		printf("inheri: c1.pri=%d\n", pri_value); // compile error
		printf("inheri: c1.pro=%d\n", pro_value);
	}
};

Parent1 p1(100, 200, 300);
Child1 c1(10, 20, 30);

class P1_friend {
public:
	// Parent1のfriendは、public, private, protected にアクセス可能
	void func(void)
	{
		printf("friend: p1.pub=%d\n", p1.pub_value);
		printf("friend: p1.pri=%d\n", p1.pri_value);
		printf("friend: p1.pro=%d\n", p1.pro_value);
	}
};

P1_friend p1f;

int main(void)
{
	// Child1, Parent1 の外部からは、publicのみアクセス可能
	printf("main  : c1 pub=%d\n", c1.pub_value);
//	printf("main  : pri=%d\n", c1.pri_value); // compile error
//	printf("main  : pro=%d\n", c1.pro_value); // compile error

	printf("main  : p1.pub=%d\n", p1.pub_value);
//	printf("main  : p1.pri=%d\n", p1.pri_value); // compile error
//	printf("main  : p1.pro=%d\n", p1.pro_value); // compile error

	c1.func(); // c1はp1を継承

	p1f.func(); // p1fはp1のフレンド

	return 0;
}
