
class Parent2 {
	void func2() { System.out.println("Parent2: func2"); }
}

// abstract classは通常のclassを継承可能
// abstractなメンバがあるclassは、abstractとして定義する
abstract class Parent1 extends Parent2 {
	abstract void func2(); // 継承元のfunc2を抽象化できる

	// abstract classにfinalを実装可能
	final void func() {
		System.out.println("func");
	}
}

class Hoge extends Parent1 {
	void func2() { // func2を実装しないとcompile error
		System.out.println("Hoge: func2");
	}
	// compile error: finalをオーバーライドできない
	//void func() { System.out.println("Hoge: func"); }
}

class Main {
	public static void main(String[] args) {
		Parent1 p = new Hoge();
		// compile error: abstract のインスタンスは作成できない
		//Parent1 p1 = new Parent1();

		p.func();  // func
	 	p.func2(); // Hoge: func2

	}
}

