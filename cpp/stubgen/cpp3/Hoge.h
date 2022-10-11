#include <string>

extern const char *Str;

class Hoge {

public:
	Hoge();
	Hoge(int n);
	~Hoge();
	int m_func(int n);
	static void s_func(char (&buf)[16]);

	int num = 789;
};

extern bool
		  g_strcmp(std::string s1, 
				   std::string s2);

namespace	 	nsHoge 
{ 	

 bool g_strcmp2(std::string s1, std::string s2);

 namespace nsHoge2 {
	Hoge *func2(const char *s, int &n);
 };

 namespace nsHoge3 {
	Hoge &func3
	  (
			 const 	char
			  *s, int 	&n);
 };

 extern int Num[8];
};

