#include <stdio.h>
#include <stdexcept>
#include <cstring>

class Hoge {
public:
	Hoge(const char *s)
	{
		if(s == NULL)
			throw std::invalid_argument("s is NULL");
		if(strlen(s) > 10)
			throw std::out_of_range("s is too long");

		printf("%s is OK\n", s);
	}
	~Hoge() {}
};

Hoge *Init(const char *s)
{
	Hoge *h = NULL;

	try {
		h = new Hoge(s);
		// Hogeのコンストラクタでthrow すると、catch から再開
	}
	catch (std::exception &e) {
		printf("new() fail: %s\n", e.what());
	}
	return h;
}

int main(void)
{
	Init(NULL);
	Init("tooooooooooooooooooooo long string");
	Init("Safe");
}

