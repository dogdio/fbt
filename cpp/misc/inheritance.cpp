#include <stdio.h>
// Parentのデストラクタがvirtualでない場合、
// Childのデストラクタは呼ばれない
//
// 順番は逆になる
//   コンストラクタ: Parent --> Child
//   デストラクタ  : Child --> Parent
class Parent1 {
public:
	Parent1()  { printf("Parent1 init\n"); }
	~Parent1() { printf("Parent1 exit\n"); }
};

class Parent2 {
public:
	Parent2() { printf("Parent2 init\n"); }
	virtual ~Parent2() { printf("Parent2 exit\n"); }
};

class Child1 : public Parent1 {
public:
	Child1()  { printf("Child1 init\n"); }
	~Child1() { printf("Child1 exit\n"); }
};

class Child2 : public Parent2 {
public:
	Child2()  { printf("Child2 init\n"); }
	~Child2() { printf("Child2 exit\n"); }
};

int main(void)
{
	Parent1 *p1 = new Child1(); // Parent1, Child1 init
	Parent2 *p2 = new Child2(); // Parent2, Child2 init

	printf("\n### delete p1\n");
	delete p1; // Parent1 exit

	printf("\n### delete p2\n");
	delete p2; // Child2, Parent2, exit

	return 0;
}
