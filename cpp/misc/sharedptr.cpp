#include <stdio.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <functional>

class SharedData {
public:
	SharedData()  { printf("### S:Start %p\n", this); }
	~SharedData() { printf("### S:End   %p\n\n", this); }
	int num;
	std::string str;

	// shared_ptr では以下の関数はすべて不要
	SharedData (const SharedData& rv) = delete; // Copy Constructor
	SharedData& operator = (const SharedData& rv) = delete; // Copy Assignment Operator
	SharedData (SharedData&& rv) = delete; // Move Constructor
	SharedData& operator = (SharedData&& rv) = delete; // Move Assignment Operator
};
using SHARED_DATA = std::shared_ptr<SharedData>;
using CALLBACK    = std::function<void(void)>;

SHARED_DATA get_shared(int n, const char *s)
{
	SHARED_DATA ret = std::make_shared<SharedData>();
	ret->num = n;
	ret->str = s;
	return ret;
}

void touch_shared(SHARED_DATA data)
{
	// この関数にshared_ptrが渡されるので、参照カウンタが+1される
	printf("touch: [%p] count=%ld, %s => %d\n", 
		data.get(), data.use_count(), data->str.c_str(), data->num);
}

void test(CALLBACK finished)
{
	SHARED_DATA data = get_shared(4444, "C is good syntax");
	printf("test : [%p] count=%ld, %s => %d\n", 
		data.get(), data.use_count(), data->str.c_str(), data->num);

	// shared_ptrの場合
	//   キャプチャをコピーにしてもコピーコンストラクタは走らない
	//auto func = [&]() // SEGV!!
	auto func = [=]()
	{
		sleep(1);// test()を確実に抜けるまで待つ

		printf("1: count=%ld\n", data.use_count());
		SHARED_DATA tmp = data; // 参照カウンタが増える
		printf("2: count=%ld\n", data.use_count());

		touch_shared(data);
		touch_shared(tmp);
		printf("3: count=%ld\n", data.use_count());

		finished(); // コールバックの後にSharedDataのデストラクタが走る
	};
	std::thread thr1(func);
	thr1.detach();
}

int main(int argc, char **argv)
{
	bool loop = true;
	test( [&]{ printf("loop END\n"); loop = false; } );

	printf("<<<<<<<<<< detach END (out of scope)\n\n");
	while(loop)
		sleep(1); // test()コンテキストが破棄された状態で待つ

	printf("<<<<<<<<<< main END \n");
	return 0;
}
