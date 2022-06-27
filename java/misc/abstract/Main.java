
abstract class AbstractHoge {
	// compile error: abstractされた関数は実装できない
	// abstract void func2(int x) { }
	abstract void func2();

	// compile error: abstract+static の組み合わせは不可能
	// interfaceの場合はstaticを実装可能(オーバーライドも可能)
	//abstract static void func3() { }

	void func() {
		System.out.println("abstract func");
	}
}

//class Hoge implements AbstractHoge { // compile error
class Hoge extends AbstractHoge {
	void func2() { // func2を実装しないとcompile error
		System.out.println("Hoge: func2");
	}
	void func() {
		System.out.println("Hoge: func");
	}
}
class Hoge2 extends AbstractHoge {
	void func2() { // func2を実装しないとcompile error
		System.out.println("Hoge2: func2");
	}
	// abstractじゃないfuncを実装するかは自由
}

class Main {
	public static void main(String[] args) {
		AbstractHoge h = new Hoge();
		AbstractHoge h2 = new Hoge2();
		// compile error: abstract のインスタンスは作成できない
		//AbstractHoge ah = new AbstractHoge();

		h.func();  // Hoge: func
	 	h.func2(); // Hoge: func2

		// オーバーラードしていないときは、abstract classのfuncが呼ばれる
		h2.func();  // abstract func
		h2.func2(); // Hoge2: func2
	}
}

