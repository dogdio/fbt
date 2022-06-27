package Hoge;

// class宣言時にpublicを省略すると、外部からimport不可となる
// packet内部からは利用可能
class HogeInst {
	public static void Func(){

		Parent p = new Parent();
		//System.out.println("Hoge:Inst.PriNum=" + p.PriNum); // compile error
		System.out.println("Hoge:Inst.PubNum=" + p.PubNum);
		System.out.println("Hoge:Inst.ProNum=" + p.ProNum);
		System.out.println("Hoge:Inst.DefNum=" + p.DefNum);

		HogeChild hc = new HogeChild();
		hc.Func();
	}
}
// package内部 Parentインスタンス化
//   Parentの public,protected,default にアクセス可能
