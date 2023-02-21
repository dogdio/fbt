#include <stdio.h>

extern int not_found;
int hoge(void)
{
	return not_found++;
}
