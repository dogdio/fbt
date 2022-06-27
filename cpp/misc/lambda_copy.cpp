#include <stdio.h>
#include <functional>
#include <thread>

class TestArgs {
public:
	TestArgs()  { printf("C : Start %p\n", this); }
	~TestArgs() { printf("D : End   %p\n", this); }
	TestArgs(const TestArgs& rv) {
		printf("CC: Start %p\n", this);
		Num = rv.Num;
	}
	int Num;
};

void any_proc(std::function<void(void)> callback)
{
	printf("  START: any proc \n");
	callback();
	printf("  END: any proc \n");
}

// ラムダ式 キャプチャ=コピーの例
//   コンストラクタx1, コピーコンストラクタx3
//   デストラクタx4 が発生する
void lambda_copy(void)
{
	TestArgs Args; // invoke: Constructor
	Args.Num = 8888;
	printf("Args=%p, Num=%d\n", &Args, Args.Num);

	{
		printf("\n+++++ \n");
		// invoke: Copy Constructor
		auto func = [=](void) // capture [copy]
		{
			printf("    Args=%p, Num=%d\n", &Args, Args.Num);
			//Args.Num++; // compile error. 'caputure copy' is read-only
		};

		printf("===== \n");
		// invoke: Copy Constructor x2
		any_proc(func);
		// invoke: Destructor x2
		printf("----- \n\n");
	}
	// invoke: Destructor(end of bracket scope)
}
// invoke: Destructor(end of function)

int main(int argc, char **argv)
{
	lambda_copy();
	return 0;
}
