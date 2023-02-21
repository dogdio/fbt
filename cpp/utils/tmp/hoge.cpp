#include <stdio.h>

int hoge(void)
{
	static int num = 0;
	return num++;
}
