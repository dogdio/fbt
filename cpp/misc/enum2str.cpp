#include <stdio.h>

enum {
	HIDEBU,
	TAWABA,
	UWARABA,
	AMIBA,
};

#define HOGEHOGE 1

#define ENUM2STR(A) #A
int main(void)
{
	printf("%s\n", ENUM2STR(HIDEBU));
	printf("%s\n", ENUM2STR(HOGEHOGE));
	return 0;
}



