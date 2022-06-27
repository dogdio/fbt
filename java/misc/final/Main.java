final class CanNotInherit {

}
// compile error: finalクラスは継承できない
//class CompileError extends CanNotInherit {
//	public int num;
//}

class Parent {
	// 関数にfinalをつけた場合はオーバーライド不可となる
	final void func(){
		System.out.println("final");
	}
}
class Hoge extends Parent {
	public int num;

	// constanceな値にはfinalをつける
	//final public int fnum; // compile error: 初期値なしはダメ
	final public int fnum = 123;

	// compile error: 
	//void func() { }
}

class Main {
	public static void main(String[] args) {
		Hoge h = new Hoge();

		h.num++;
		//h.fnum++; // compile error: finalメンバ変数は変更不可能

		h.func();
	}
}

