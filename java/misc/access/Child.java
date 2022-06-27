import Hoge.Parent;
//import Hoge.HogeInst; // compile error

public class Child extends Parent {
	private int pri_num = 100;
	public int pub_num = 200;
	protected int pro_num = 300;
	int def_num = 400;

	public void Print() {
		System.out.println("EX:Child.pri_num=" + pri_num);
		System.out.println("EX:Child.pub_num=" + pub_num);
		System.out.println("EX:Child.pro_num=" + pro_num);
		System.out.println("EX:Child.def_num=" + def_num);

		//System.out.println("EX:Parent.PriNum=" + PriNum); // compile error
		System.out.println("EX:Parent.PubNum=" + PubNum);
		System.out.println("EX:Parent.ProNum=" + ProNum);
		//System.out.println("EX:Parent.DefNum=" + DefNum); // compile error
	}
}
// package外部 Parent継承
//   Parentの public,protected にアクセス可能

