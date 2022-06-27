
class Hoge {
	// ビルドは通るが、、、引数のx,yが更新されるだけ
	// メンバ変数とはみなされない
	void func2(int x, int y) {
		x = x;
		y = y;
	}
	// thisをつければメンバ変数であることを識別可能
	void func(int x, int y) {
		this.x = x;
		this.y = y;
	}
	void print() {
		System.out.println("x=" + x  + ", y=" + y);
	}

	// staticメソッドからthisにはアクセスできない
	// 引数としてインスタンスを渡してやれば大丈夫
	static public void func3(Hoge This, int x, int y) {
		This.x = x;
		This.y = y;
		//this.y = y; // compile error
	}

	private int x;
	private int y;
}

class Main {
	public static void main(String[] args) {
		Hoge h = new Hoge();
		h.func(100, 200);
		h.print();

		h.func2(300, 400);
		h.print();

		Hoge.func3(h, 300, 400);
		h.print();
	}
}

