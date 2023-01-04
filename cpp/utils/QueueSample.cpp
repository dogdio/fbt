#include <stdio.h>
#include "Queue.h"

using namespace Utils;

class Hoge {
public:
	Hoge() { printf("start\n"); }
	~Hoge() { printf("stop\n"); }
	//Hoge (const Hoge& rv) = delete; // Compile error
#if 1
	Hoge (const Hoge& rv) { printf("copy\n"); num = rv.num; }
	Hoge& operator=(const Hoge& rv) {
		printf("copy assign\n");
		if(this != &rv) {
			num = rv.num;
		}
		return *this;
	}
#endif
	int num = 10;
};

typedef struct Hoge2 {
	int v;
	Hoge h;
}HOGE2;

typedef struct Hoge3 {
	int v;
	Hoge *h;
}HOGE3;

typedef struct Hoge4 {
	int v;
	std::shared_ptr<Hoge> h;
}HOGE4;

using HOGE5 = std::shared_ptr<Hoge>;

// クラス変数をqueueにpush/popする
void test1(void)
{
	printf("## %s: \n", __func__);
	Hoge a;
	Hoge b;
	Queue::QueueIF<Hoge> HogeQ;

	a.num = 12345;
	// queue.push() でコピーコンストラクタが呼ばれる
	HogeQ.WakeMsg(a);
	printf("a.num=%d\n\n", a.num);

	a.num = 67890;
	HogeQ.WakeMsg(a);
	printf("a.num=%d\n\n", a.num);

	// queue.front() でコピー代入演算子が呼ばれる
	// queue.pop() でデストラクタが呼ばれる
	// 戻り値を入れるときに、コピー代入演算子が呼ばれる
	b = HogeQ.WaitMsg();
	printf("b.num=%d\n\n", b.num);

	b = HogeQ.WaitMsg();
	printf("b.num=%d\n\n", b.num);
}

// クラス変数(実体)を含む構造体をqueueにpush/popする
void test2(void)
{
	printf("## %s: \n", __func__);
	HOGE2 a;
	HOGE2 b;
	Queue::QueueIF<HOGE2> HogeQ;

	a.h.num = 12345;
	// queue.push() でコピーコンストラクタが呼ばれる
	HogeQ.WakeMsg(a);
	printf("a.h.num=%d\n\n", a.h.num);

	a.h.num = 67890;
	HogeQ.WakeMsg(a);
	printf("a.h.num=%d\n\n", a.h.num);

	// queue.front() でコピー代入演算子が呼ばれる
	// queue.pop() でデストラクタが呼ばれる
	// 戻り値を入れるときに、コピー代入演算子が呼ばれる
	b = HogeQ.WaitMsg();
	printf("b.h.num=%d\n\n", b.h.num);

	b = HogeQ.WaitMsg();
	printf("b.h.num=%d\n\n", b.h.num);
}

// クラス変数(ポインタ)を含む構造体をqueueにpush/popする
// ポインタ渡しなので、コンストラクタ等は走らない
void test3(void)
{
	printf("## %s: \n", __func__);
	HOGE3 a;
	HOGE3 b;
	Queue::QueueIF<HOGE3> HogeQ;

	a.h = new Hoge();
	a.h->num = 12345;
	a.v = 1000;
	HogeQ.WakeMsg(a);
	printf("a.h.num=%d, a.v=%d,\n\n", a.h->num, a.v);

	a.h->num = 67890;
	a.v = 2000; // ここは個別に確保される
	HogeQ.WakeMsg(a);
	printf("a.h.num=%d, a.v=%d,\n\n", a.h->num, a.v);

	// ポインタがそのまま格納されるので、最後の値(67890)が2回くる
	b = HogeQ.WaitMsg();
	printf("b.h.num=%d, b.v=%d, \n\n", b.h->num, b.v);

	b = HogeQ.WaitMsg();
	printf("b.h.num=%d, b.v=%d, \n\n", b.h->num, b.v);

	delete(a.h);
}

// クラス変数(sharedポインタ)を含む構造体をqueueにpush/popする
// ポインタ渡しなので、コンストラクタ等は走らない
void test4(void)
{
	printf("## %s: \n", __func__);
	HOGE4 b;
	Queue::QueueIF<HOGE4> HogeQ;
	{
		HOGE4 a;
    
		a.h = std::make_shared<Hoge>();
		a.h->num = 12345;
		a.v = 1000;
		HogeQ.WakeMsg(a);
		printf("a.h.num=%d, a.v=%d,\n\n", a.h->num, a.v);
    
		a.h->num = 67890;
		a.v = 2000; // ここは個別に確保される
		HogeQ.WakeMsg(a);
		printf("a.h.num=%d, a.v=%d,\n\n", a.h->num, a.v);
	} // shared pointerはこの後も有効

	// ポインタがそのまま格納されるので、最後の値(67890)が2回くる
	b = HogeQ.WaitMsg();
	printf("b.h.num=%d, b.v=%d, \n\n", b.h->num, b.v);

	b = HogeQ.WaitMsg();
	printf("b.h.num=%d, b.v=%d, \n\n", b.h->num, b.v);

	// 関数を抜けるとデストラクタが走る
}

// sharedポインタをqueueにpush/popする
// ポインタ渡しなので、コンストラクタ等は走らない
void test5(void)
{
	printf("## %s: \n", __func__);
	HOGE5 b;
	Queue::QueueIF<HOGE5> HogeQ;
	{
		HOGE5 a;
    
		a = std::make_shared<Hoge>();
		a->num = 12345;
		HogeQ.WakeMsg(a);
		printf("a.num=%d\n\n", a->num);
    
		a->num = 67890;
		HogeQ.WakeMsg(a);
		printf("a.num=%d\n\n", a->num);
	} // shared pointerはこの後も有効

	// ポインタがそのまま格納されるので、最後の値(67890)が2回くる
	b = HogeQ.WaitMsg();
	printf("b.num=%d\n\n", b->num);

	b = HogeQ.WaitMsg();
	printf("b.num=%d\n\n", b->num);

	// 関数を抜けるとデストラクタが走る
}

int main(int argc, char **argv)
{
	if(argc > 1) {
		switch(atoi(argv[1])) {
			case 1: test1(); break; // class value
			case 2: test2(); break; // struct + class value
			case 3: test3(); break; // struct + class pointer
			case 4: test4(); break; // struct + class shared pointer
			case 5: test5(); break; // class shared pointer
		}
	}
	printf("END\n");
	return 0;
}
