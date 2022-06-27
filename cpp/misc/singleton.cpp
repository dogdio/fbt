#include <stdio.h>

class Single {
public:
	static Single *GetInstance(void); // must be static
	void Func(void) { printf("Single\n"); }

private:
	Single(const char *s) { printf(">>> Start %s\n", s); str = s; }
	~Single()             { printf("<<< End %s\n", str); }
	const char *str;
};
//static Single s2("Global Static"); // compile error

Single *Single::GetInstance(void)
{
	// この関数が呼ばれた時点でインスタンス生成される
	// インスタンスの破棄は、main()終了後になる
	static Single s("Local Static");
	return &s;
}

int main(void)
{
	printf("main start\n");
#if 0 // シングルトンを外部からインスタンス生成できない
	Single s; // compile error
	Single *s = new Single(); // compile error
#endif
	Single *s = Single::GetInstance();
	s->Func();
	Single::GetInstance()->Func(); //こういう書き方もOK

	printf("main end\n");
	return 0;
}
