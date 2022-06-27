#include <stdio.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <functional>

class UniqueData {
public:
	UniqueData()  { printf("### U:Start %p\n", this); }
	~UniqueData() { printf("### U:End   %p\n\n", this); }
	int num;
	std::string str;

	// 別の変数に渡すときはstd::moveが必要だが、以下の関数はなくても大丈夫らしい。。
	UniqueData (const UniqueData& rv) = delete; // Copy Constructor
	UniqueData& operator = (const UniqueData& rv) = delete; // Copy Assignment Operator
	UniqueData (UniqueData&& rv) = delete; // Move Constructor
	UniqueData& operator = (UniqueData&& rv) = delete; // Move Assignment Operator
};
using UNIQUE_DATA = std::unique_ptr<UniqueData>;
using CALLBACK    = std::function<void(void)>;

UNIQUE_DATA get_unique(int n, const char *s)
{
	UNIQUE_DATA ret(new UniqueData);
	ret->num = n;
	ret->str = s;
	return ret;
}

void touch_unique(UNIQUE_DATA data)
{
	printf("touch: [%p] %s => %d\n", data.get(), data->str.c_str(), data->num);
	// この関数を抜けるときに、unique_ptrが開放される(誰も所有していない状態)
}

void test(CALLBACK finished)
{
	{
		UNIQUE_DATA data = get_unique(55555, "GO is stupid syntax");
		UNIQUE_DATA data2 = std::move(data);
		// std::moveを使って渡す。コンストラクタ、デストラクタは走らない

		//touch_unique(data); // compile error
		//touch_unique(std::move(data)); // SEGV: data はすでに開放済み
		touch_unique(std::move(data2));
		printf("1:unique pointer already delete\n\n");
	}

	{
		UNIQUE_DATA data = get_unique(666666, "C++ is terrible syntax");
   
		// キャプチャ(=)でローカルなunique_ptrを渡すことはできない
		// std::moveで引数として渡さないとダメ
		// auto func = [&]() // ビルドは通るがSEGVになる
		auto func = [=](UNIQUE_DATA data)
		{
			sleep(1);// test()を確実に抜けるまで待つ

			touch_unique(std::move(data));
			printf("2:unique pointer already delete\n\n");
			finished();
		};
		std::thread thr1(func, std::move(data));
		thr1.detach();
	}
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
