package sample;
import sample.Hoge.*;

public class TestHoge {
	public static void test_is_zero()
	{
		Hoge h = new Hoge();
		System.out.println("ret: " + h.is_zero(0));
		System.out.println("ret: " + h.is_zero(1));
	}

	public static void test_is_zero_zero()
	{
		Hoge h = new Hoge();
		System.out.println("ret: " + h.is_zero_zero(0, 0));
//		System.out.println("ret: " + h.is_zero_zero(1, 1));
		System.out.println("ret: " + h.is_zero_zero(0, 1));
		System.out.println("ret: " + h.is_zero_zero(1, 0));
	}

	public static void test_is_zero_zero2()
	{
		Hoge h = new Hoge();
		System.out.println("ret: " + h.is_zero_zero2(0, 0));
		System.out.println("ret: " + h.is_zero_zero2(1, 1));
		System.out.println("ret: " + h.is_zero_zero2(0, 1));
//		System.out.println("ret: " + h.is_zero_zero2(1, 0));
	}

	public static void test_is_zero_zero3()
	{
		Hoge h = new Hoge();
//		System.out.println("ret: " + h.is_zero_zero3(0, 0));
		System.out.println("ret: " + h.is_zero_zero3(1, 1));
		System.out.println("ret: " + h.is_zero_zero3(0, 1));
		System.out.println("ret: " + h.is_zero_zero3(1, 0));
	}

	public static void main(String[] args) {
		test_is_zero();
		test_is_zero_zero();
		test_is_zero_zero2();
		test_is_zero_zero3();
	}
}

