package sample;

public class Hoge {
	public boolean is_zero(int arg)
	{
		if(arg == 0)
			return true;
		else
			return false;
	}

	public boolean is_zero_zero(int arg, int arg2)
	{
		if(arg == 0 && arg2 == 0)
			return true;
		else
			return false;
	}

	public boolean is_zero_zero2(int arg, int arg2)
	{
		if(arg != 0 || arg2 != 0)
			return false;
		else
			return true;
	}

	public boolean is_zero_zero3(int arg, int arg2)
	{
		if(arg == 0) {
			if(arg2 == 0)
				return true;
			else
				return false;
		}
		else
			return false;
	}
}
