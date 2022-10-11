#include <stdio.h>
#include "Hoge.h"

namespace nsHoge {

void Hoge::Add(int key, std::map<std::string, int> list) {
	Lists[key] = list;
}

std::map<std::string, int> &Hoge::GetMap(int key)
{
	return Lists[key];
}

const char *Str = "Hoge";

namespace nsHoge2 {

bool g_strcmp(std::string s1, std::string s2)
{
	if(s1 == s2)
		return true;
	else
		return false;
}

}

}

nsHoge::STRCMP_RET g_strcmp2(std::string s1, std::string s2)
{
	if(s1 == s2)
		return nsHoge::OK;
	else
		return nsHoge::NG;
}
