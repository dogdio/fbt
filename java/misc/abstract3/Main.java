
abstract class Parent2 {
	abstract void func3();
}

abstract class Parent1 extends Parent2 {
	abstract void func2();
	//abstract void func3(); // あってもなくてもいい

	void func() {
		System.out.println("func");
	}
}

class Hoge extends Parent1 {
	void func2() { // func2を実装しないとcompile error
		System.out.println("Hoge: func2");
	}
	void func3() { // func3を実装しないとcompile error
		System.out.println("Hoge: func3");
	}
}

class Main {
	public static void main(String[] args) {
		Parent1 p = new Hoge();
		// compile error: abstract のインスタンスは作成できない
		//Parent1 p1 = new Parent1();

		p.func();  // func
	 	p.func2(); // Hoge: func2
	 	p.func3(); // Hoge: func3

	}
}

