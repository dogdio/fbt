//
// javaにデストラクタはない。基本的にgcに任せる
// gcだとスコープ外になったときに、変数の開放タイミングを保証できない
// (デストラクタが呼ばれるタイミングが不定ならば、ないほうがマシってこと)
class Hoge {
	Hoge(String s) {
		System.out.println("constructor:" + s);
		msg = s;
	}
	public void Print() {
		System.out.println("#" + msg);
	}
	public void SetMsg(String s) {
		msg = s;
	}
	String msg;
}

class Main {
	public static void main(String[] args) {
		String s[] = {"null", "po"};

		for(int i = 0; i < args.length && i < 2; i++)
			s[i] = args[i];

		// new()により、コンストラクタが起動する(h1, h2)
		Hoge h1 = new Hoge("h1 " + s[0]);
		Hoge h2 = new Hoge("h2 " + s[1]);

		// h3のコンストラクは起動しない(ポインタ渡し)
		Hoge h3 = h2;
		h3.Print();
		h3.SetMsg("this is h3"); // h2にも同様の値がセットされる

		System.out.println("#############");
		h1.Print();
		h2.Print();
		h3.Print();
	}
}

