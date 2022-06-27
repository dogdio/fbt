class Parent1 {
	void func() { System.out.println("Parent1 func"); }
}
class Parent2 {
	void func2() { System.out.println("Parent2 func"); }
}
interface HogeIF {
	void HogeFunc();
}
interface HogeIF2 {
	void HogeFunc2();
}
abstract class HogeABST {
	abstract void HogeFunc3();
}
// Javaで多重継承は基本的に禁止されているが、、
// interfaceが複数のinterfaceを継承するのはOK(これ以外はすべてNG)
interface HogeIFs extends HogeIF, HogeIF2 {
	void HogeFunc();
	void HogeFunc2();
}
class HogeImpl implements HogeIFs {
	public void HogeFunc() { System.out.println("Hoge Func"); }
	public void HogeFunc2() { System.out.println("Hoge Func2"); }
}

//interface HogeIFs2 extends HogeIF, HogeABST { // compile error
//	void HogeFunc();
//	void HogeFunc3();
//}
//interface HogeIFs2 extends HogeIF, Parent1 { // compile error
//	void HogeFunc();
//	void func();
//}
//interface HogeIFs2 extends Parent1, Parent2 { // compile error
//	void func();
//	void func2();
//}

//abstract class AbstHoge extends HogeIF, HogeIF2 { // compile error
//	void HogeFunc();
//	void HogeFunc2();
//}

//class Hoge3 extends HogeIF, HogeIF2 { } // compile error
//class Hoge2 extends Parent1, HogeIF { } // compile error
//class Hoge extends Parent1, Parent2 { } // compile error

class Main {
	public static void main(String[] args) {
		HogeIFs h = new HogeImpl();
		h.HogeFunc();
		h.HogeFunc2();
	}
}
