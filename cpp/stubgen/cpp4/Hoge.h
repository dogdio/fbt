#include <string>
#include <map>

namespace nsHoge {

typedef enum {
	OK,
	NG
}STRCMP_RET;

class Hoge {

public:
	void Add(int key, std::map<std::string, int> list);
	std::map<std::string, int> &GetMap(int key);


private:
	std::map<int, std::map<std::string, int>> Lists;
};

 extern const	char * Str ;

 namespace nsHoge2 {
    extern bool g_strcmp(std::string s1, std::string s2);
 };

};

extern nsHoge::STRCMP_RET	g_strcmp2	(std::string	s1,std::string s2 ) ;

