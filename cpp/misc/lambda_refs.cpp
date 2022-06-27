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

// ラムダ式 キャプチャ=参照の例
//   コンストラクタx1, デストラクタx1 が発生する
//   参照の場合は余分なコンストラクタ、デストラクタは発生しない
//   ただし、参照元のコンテキストが開放された場合、ラムダ式側の動作は不定
void lambda_refs(void)
{
	TestArgs Args; // invoke: Constructor
	Args.Num = 8888;
	printf("Args=%p, Num=%d\n", &Args, Args.Num);

	{
		printf("\n+++++ \n");
		auto func = [&](void) // capture [reference]
		{
			printf("    Args=%p, Num=%d\n", &Args, Args.Num);
			Args.Num++; // writable
		};

		printf("===== \n");
		any_proc(func);
		printf("----- \n\n");
	}

	printf("Args=%p, Num=%d\n", &Args, Args.Num);
}
// invoke: Destructor(end of function)

int main(int argc, char **argv)
{
	lambda_refs();
	return 0;
}
