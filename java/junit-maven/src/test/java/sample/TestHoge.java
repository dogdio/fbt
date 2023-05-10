package sample;
import sample.Hoge.*;

// junit4
import org.junit.Test;
import static org.junit.Assert.*; // staticメンバのクラス名を省略できる

public class TestHoge {

	@Test
	public void test_is_zero()
	{
		Hoge h = new Hoge();
//		assertTrue(h.is_zero(0));
		assertFalse(h.is_zero(1));
		System.out.println("zero");
	}

	@Test
	public void test_is_zero_zero()
	{
		Hoge h = new Hoge();
		assertEquals(h.is_zero_zero(0, 0), true);
		assertEquals(h.is_zero_zero(1, 1), false);
		assertFalse(h.is_zero_zero(0, 1));
		assertFalse(h.is_zero_zero(1, 0));
		System.out.println("zero_zero");
	}

	@Test
	public void test_is_zero_zero2()
	{
		Hoge h = new Hoge();
		assertTrue(h.is_zero_zero2(0, 0));
		assertFalse(h.is_zero_zero2(1, 1));
//		assertFalse(h.is_zero_zero2(0, 1));
//		assertFalse(h.is_zero_zero2(1, 0));
		System.out.println("zero_zero2");
	}

	@Test
	public void test_is_zero_zero3()
	{
		Hoge h = new Hoge();
//		assertTrue(h.is_zero_zero3(0, 0));
		assertFalse(h.is_zero_zero3(1, 1));
		assertFalse(h.is_zero_zero3(0, 1));
		assertFalse(h.is_zero_zero3(1, 0));
		System.out.println("zero_zero3");
	}
}

