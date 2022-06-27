#include <stdio.h>
#include <sstream>
#include <thread>

int num = 0;
void Hoge(void)
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	printf("[%d] tid=%llx\n", num++, std::stoull(ss.str()));
}

class HogeHoge {
public: // staticじゃないメソッドはstd::threadに渡せない
	static void Func(void) { Hoge(); }
	       void Func2(void) { Hoge(); }
};

int main(int argc, char **argv)
{
	// basic function
	std::thread thr(Hoge);

	// anonymous function 1
	std::thread thr2([](){ Hoge(); });

	// anonymous function 2
	auto func = []() {
		Hoge();
	};
	std::thread thr3(func);

	// static class method
	std::thread thr4(HogeHoge::Func);
	//std::thread thr5(HogeHoge::Func2); // compile error

	thr.join();
	thr2.join();
	thr3.join();
	thr4.join();
	return 0;
}

