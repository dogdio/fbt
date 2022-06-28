#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool get_true(void)
{
	return true;
}

bool is_zero(int arg)
{
	if(arg == 0)
		return true;
	else
		return false;
}

bool is_zero_zero(int arg, int arg2)
{
	if(arg == 0 && arg2 == 0)
		return true;
	else
		return false;
}

bool is_zero_zero_2(int arg, int arg2)
{
	if(arg != 0 || arg2 != 0)
		return false;
	else
		return true;
}

bool is_zero_zero_3(int arg, int arg2)
{
	if(arg == 0) {
		if(arg2 == 0)
			return true;
		else
			return false;
	}
	else {
		return false;
	}
}

bool is_zero_zero_4(int arg, int arg2)
{
	if(arg != 0)
		return false;
	if(arg2 != 0)
		return false;

	return true;
}

