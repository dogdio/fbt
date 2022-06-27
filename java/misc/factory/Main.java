class Hoge {
	private Hoge() {
		System.out.println("init");
	}
	public void func() {
		System.out.println("Hoge func:" + num);
	}
	public void Set(int n) {
		num = n;
	}
	public static void Dummy() {
		System.out.println("## don't douch 'Inst'");
	}

	// static関数を外部から読んだときに、メンバ変数Instがnewされる
	// static領域を返す場合はシングルトンになる
	public static Hoge GetInstance() {
		System.out.println("## after Hoge Constructor");
		return Inst;
	}
	// main()が呼ばれた時点ではまだnewされていない
	private static Hoge Inst = new Hoge();
	private int num;
}

// staticメンバの初期化タイミング
// static関数などを外から読んだときに発生する?
class Main {
	public static void main(String[] args) {
		System.out.println(">>>>> main() >>>>>");
		if(args.length == 1 && args[0].equals("xxx"))
			Hoge.Dummy();// これを読んだ時点でコンストラクタが起動する

		Hoge h = Hoge.GetInstance();
		h.Set(123);
		h.func();

		Hoge.GetInstance().Set(456); // 同じインスタンスが得られる
		h.func();

		// compile error
		// コンストラクタがprivateなので外からnewできない
		//Hoge h2 = new Hoge();
	}
}

