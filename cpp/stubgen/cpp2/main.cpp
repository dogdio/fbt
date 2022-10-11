#include <stdio.h>
#include "Hoge.h"

int main(void)
{
	Hoge *h2 = new Hoge(300);
	Hoge h;
	int n = nsHoge::Num[4];
	char buf[16] = "123456789012345";

	n = h.m_func(n);
	Hoge::s_func(buf);

	nsHoge::nsHoge3::func3("nsHoge::func3", h2->num);
	nsHoge::nsHoge2::func2("nsHoge::func2", h2->num);

	printf("ret=%d\n", g_strcmp("12345", "12346"));
	printf("ret=%d\n", nsHoge::g_strcmp2("12345", Str));

	return 0;
}
