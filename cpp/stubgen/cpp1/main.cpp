#include <stdio.h>
#include "Hoge.h"

int main(void)
{
	Hoge *h2 = new Hoge(300);
	Hoge h;
	int n = 100;

	n = h.m_func(n);
	Hoge::s_func(h.num + n);
	Hoge::s_func();

	nsHoge::nsHoge2::func2("nsHoge::func2", h2->num);
	nsHoge::nsHoge3::func3("nsHoge::func3", h2->num);

	printf("ret=%d\n", g_strcmp("12345", "12346"));

	return 0;
}
