package Sample;
import IF.*;

public class Hoge2 implements HogeIF {
	public int Add(int x, int y) {
		return 10 * (x + y);
	}
	public String OwnName() {
		return "x10";
	}
	private Hoge2() {
		Factory.AddInstance("x10", this);
	}
	static public void Touch() {
		System.out.println("Hoge2 Touch");
	}
	static private final Hoge2 Inst = new Hoge2();
}

