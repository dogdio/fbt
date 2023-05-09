import Hoge.Parent;
// アクセス修飾子 まとめ 
//   private:
//     クラス内部のみで使用する
//   public:
//     誰でも使用可能
//   default:
//     packageの外部に見せたくないが、package内部で共通
//   protected:
//     継承したクラスに見せたいが、継承していないクラスには見せたくない
//
// package  class   |    accessible    |  source code
//------------------+------------------+---------------
//   in      in     | pri pub pro def  | Parent.java
//   in      ex     |     pub pro def  | HogeInst.java
//   in     inherit |     pub pro def  | HogeChild.java
//   ex      in     | ~~~~~~~~~~~~~~~  | ~~~~~~~~~~~~~~
//   ex      ex     |     pub          | Main.java
//   ex     inherit |     pub pro      | Child.java

public class Main {
	private static void ExternalPackage_ChildInstance() {
		Child c = new Child();
		System.out.println("EX:c.pub_num=" + c.pub_num);
		System.out.println("EX:c.pro_num=" + c.pro_num);
		System.out.println("EX:c.def_num=" + c.def_num);
		//System.out.println("EX:c.pri_num=" + c.pri_num); // compile error

		System.out.println("EX:c.PubNum=" + c.PubNum);
		//System.out.println("EX:c.PriNum=" + c.PriNum); // compile error
		//System.out.println("EX:c.ProNum=" + c.ProNum); // compile error
		//System.out.println("EX:c.DefNum=" + c.DefNum); // compile error
		System.out.println("");
        
		c.Print();
		System.out.println("");
	}

	private static void ExternalPackage_ParentInstance() {
		Parent cp = new Child();
		System.out.println("EX:cp.PubNum=" + cp.PubNum);
		//System.out.println("EX:cp.PriNum=" + cp.PriNum); // compile error
		//System.out.println("EX:cp.ProNum=" + cp.ProNum); // compile error
		//System.out.println("EX:cp.DefNum=" + cp.DefNum); // compile error

		System.out.println("");
		cp.CallPriv();
	}

	public static void main(String[] args) {
		ExternalPackage_ChildInstance();
		ExternalPackage_ParentInstance();
		// SEGV: compileはできるが間違っているｗ
		//Child c2 = (Child)new Parent();
	}
}

