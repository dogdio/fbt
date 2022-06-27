class Hoge2 {
	Hoge2(String s) {
		System.out.println("Hoge2 constructor:" + s);
		msg = s;
	}
	public void Print() {
		System.out.println("#" + msg);
	}
	private String msg;
}
class Hoge {
	Hoge(String s) {
		System.out.println("Hoge constructor:" + s);
		msg = s;
	}
	public void Print() {
		System.out.println("#" + msg);
	}
	private String msg;
}

// staticクラス変数を定義時にnew()すると、main()の前にコンストラクタが呼ばれる
class Main {
	public static void main(String[] args) {
		Print2(); // これはOK(すでに割当済み)

		if(args.length == 1 && args[0].equals("xxx")) {
			System.out.println("<<<<<<< will be crash");
			Print(); // SEGV: shがまだ割り当てられてない
		}
		sh = new Hoge("static hoge");
		Print();
	}
	public static void Print() { // Print()は'static'に存在する
		sh.Print();
	}
	public static void Print2() {
		sh2.Print();
	}

	static Hoge sh; // スコープが'static'なだけでインスタンスはまだ未生成
	static Hoge2 sh2 = new Hoge2("aaa"); // Hoge2インスタンスを生成
}

