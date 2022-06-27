class Parent {
	private void priv_func(int num) {
		System.out.println("Parent priv_func: num=" + num);
	}
	// Parentインスタンスから呼び出すためには、オーバーライド対象はpublicにする
	public void func() {
		System.out.println("Parent func");
	}
	final public void final_func() { // finalをつけるとオーバーライドできない
		System.out.println("Parent final_func");
		priv_func(123);
	}
}
class Child extends Parent {
	private void priv_func(int num) { // いちおうオーバーライドはできるが、、class外からは呼べない
		System.out.println("Child priv_func:" + num);
	}
	//@Override public void func() { // @Override アノテーションあり
	public void func() { // @Override なしでも動く
		System.out.println("Child func");
	}
	public void func2() {
		System.out.println(">>>>");
		func(); // Child func
		super.func(); // Parent func
		final_func();
		priv_func(456); // privateメソッドをオーバーライドしてChild内で呼ぶ意味ないｗ
		//super.priv_func(789); // compile error
		System.out.println("<<<<");
	}
	// compile error
	//public void final_func(){ System.out.println("Child final_func"); }
}

class Main {
	public static void main(String[] args) {
		Child c = new Child();
		Parent p = c;

		//p.func2(); // compile error: オーバーライドしていない子の関数は呼べない
		//p.priv_func(); // compile error: class外からprivateメンバはアクセス不可
		//c.priv_func(); // compile error: class外からprivateメンバはアクセス不可
		p.func(); // Child func: Polymorphism
		c.func2();
	}
}

