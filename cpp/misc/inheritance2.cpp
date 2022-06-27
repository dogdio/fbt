#include <stdio.h>

class Parent1 {
public:
	virtual void Func(void)  { printf("Parent1 Func\n"); }
};

class Parent2a : public Parent1 {
public:
	virtual void Func(void) { printf("Parent2a Func\n"); }
	void Func2a(void)       { printf("Parent2a Func2a\n"); }
};
class Parent2b : public Parent1 {
public:
	void Func(void)   { printf("Parent2b Func\n"); }
	void Func2b(void) { printf("Parent2b Func2b\n"); }
};

class Child1 : public Parent2a {
public:
	void Func(void)  { printf("Child1 Func\n"); }
};
class Child2 : public Parent2b {
public:
	void Func(void) { printf("Child2 Func\n"); }
};
class Child3 : public Parent2b {

};
// 2段階の継承
//   Parent1 <-- Parent2a <-- Child1
//   Parent1 <-- Parent2b <-- Child2
//   Parent1 <-- Parent2b <-- Child3
// Parent1.Funcがvirtualであれば、Parent2a,2bがどちらの場合も、
// Child1,2のFuncが呼ばれる。
int main(void)
{
	Parent1 *p1 = new Child1(); // Parent2a.Func is virtual
	Parent1 *p2 = new Child2(); // Parent2b.Func is not virtual
	Parent1 *p3 = new Child3(); // Func実装なし
	p1->Func(); // Child1 Func
	p2->Func(); // Child2 Func
	p3->Func(); // p2b Func

	// Parentインスタンスから子クラスの(virtualでない)関数は呼べない
	//p1->Func2a(); // compile error
	//p2->Func2b(); // compile error

	// 親から子供のキャストは、一応可能だが...
#if 0 // not recommended
	printf("\n");
	Parent2a *p2a = (Parent2a *)p3;
	p2a->Func2a();
	p2a->Func();
#endif
	return 0;
}

