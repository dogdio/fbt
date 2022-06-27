// 一つのソースファイルに複数のclassを記載することが可能
// javac *.java を実行すると、そのファイル内のclassごとにオブジェクトが生成される
// オブジェクトの名前はファイル名とは関係なく、class名で決まる

class Hoge {
	public int num;
	public static int snum; // public static変数がグローバル変数的な扱い
}
// compile error: クラススコープ外で変数宣言はできない
// Hoge h_global = new Hoge();

// main関数がない*.classファイルは実行できない(ただのライブラリ扱い)
// main関数のあるclassを、java に渡すことでそこから処理が始まる
// $ java Main で実行
class Main {
	public static void main(String[] args) {
		Hoge h = new Hoge();
		Hoge h2 = new Hoge();

		// 初期値: メンバ変数はゼロクリアされている
		System.out.println("h.num=" + h.num  + ", h.snum=" + h.snum);
		System.out.println("h2.num=" + h2.num + ", h2.snum=" + h2.snum);

		// public staticメンバは、インスタンス無しでアクセス可能
		System.out.println("Hoge.snum=" + Hoge.snum);
		//System.out.println("Hoge.num=" + Hoge.num); // compile error

		System.out.println("###########");
		h.num = 10;
		h2.num = 20;
		Hoge.snum = 123;
		System.out.println("h.num=" + h.num  + ", h.snum=" + h.snum);
		System.out.println("h2.num=" + h2.num + ", h2.snum=" + h2.snum);
		System.out.println("Hoge.snum=" + new Hoge().snum); // この書き方もあり

		// compile error: staticメソッドから、publicメンバを参照することはできない
		//h3 = new Hoge();
		//System.out.println("num=" + h3.num  + ", snum=" + h3.snum);
	}

	public Hoge h3;
}

