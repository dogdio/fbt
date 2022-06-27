package Hoge;

class HogeChild extends Parent {
	public void Func(){

		//System.out.println("Hoge:Child.PriNum=" + PriNum); // compile error
		System.out.println("Hoge:Child.PubNum=" + PubNum);
		System.out.println("Hoge:Child.ProNum=" + ProNum);
		System.out.println("Hoge:Child.DefNum=" + DefNum);
	}
}
// package内部 Parent継承
//   Parentの public,protected,default にアクセス可能
