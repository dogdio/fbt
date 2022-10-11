#include <string>

extern bool
		  g_strcmp(std::string s1, 
				   std::string s2);
class Hoge {

public:
	Hoge(); ~Hoge(); Hoge(int n);

	int m_func(int n)
			;
	int num = 789;
	static
		 	void s_func	(char ( &buf)[16]) ;
};

extern const char *Str;

namespace	 	nsHoge {
 extern int Num[8];

 namespace nsHoge2 {
	Hoge *func2(const char *s, int &n);
 };

 extern bool g_strcmp2(std::string s1, std::string s2);

 namespace nsHoge3 {
	Hoge &func3
	  (
			 const 	char
			  *s, int 	&n);
 };

};

