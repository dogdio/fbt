#include <stdio.h>
#include "Hoge.h"

Hoge::Hoge(int n) {
	num = n;
}

Hoge::Hoge() { }
Hoge::~Hoge() { }

int Hoge::m_func(int n) {
	printf("member func %d\n", n);
	return n * 2;
}

void Hoge::s_func(char (&buf)[16]) {
	printf("static func %s\n", buf);
}

const char *Str = "12345";

namespace nsHoge {

 int Num[8] = {1,2,3,4,5,6,7,8};

 namespace nsHoge2 {

 Hoge *func2(const char *s, int &n) {
	static Hoge h;
	return &h;
 }

 }

 bool g_strcmp2(std::string s1, std::string s2)
 {
	if(s1 == s2)
		return true;
	else
		return false;
 }

 namespace nsHoge3 {

 Hoge &func3(const char *s, int &n) {
	static Hoge h;
	return h;
 }

 }

}

bool g_strcmp(std::string s1, std::string s2)
{
	if(s1 == s2)
		return true;
	else
		return false;
}

