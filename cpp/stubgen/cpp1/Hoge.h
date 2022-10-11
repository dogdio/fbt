#include <string>

class Hoge {

public:
	Hoge();
	Hoge (int n);
	~Hoge	();
	int m_func(int n);
	static void s_func(int	n = 	1000);

	int num = 789;
};

namespace nsHoge {
 namespace nsHoge3 {
	Hoge &func3(const char *s, int &n);
 };

 namespace nsHoge2 {
	Hoge *func2(const char *s, int &n);
 };

};

extern bool g_strcmp(std::string s1, std::string 
				 s2);
