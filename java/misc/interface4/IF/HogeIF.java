package IF;

public interface HogeIF {
	int Add(int x, int y);
	String OwnName();
	static void Touch() {}

	//static void Touch(); // compile error
	// staticの場合は、関数本体を実装しないとダメ
	// 継承先でoverrideはできる
}

