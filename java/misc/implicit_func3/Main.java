
class ParentNoArg {
	ParentNoArg() {
		System.out.println("ParentNoArg constructor:");
	}
}
class Child1 extends ParentNoArg {
	Child1(String s) {
		//super(); // が暗黙的に実施されている
		// もし、super()を書いたとしてもコンストラクタが2回走るわけではない
		System.out.println("Child1 constructor:" + s);
	}
}

class ParentWithArg {
	ParentWithArg(String s) {
		System.out.println("ParentWithArg constructor:" + s);
	}
}
class Child2 extends ParentWithArg {
	Child2(String s) {
		// 親クラスが引数ありの場合は明示的にsuper()を書く
		super(s); // 書かない場合は compile error
		System.out.println("Child2 constructor:" + s);
	}
}

// コンストラクタの起動順: Parent --> Child
class Main {
	public static void main(String[] args) {
		ParentNoArg p2 = new Child1("init hoge");
		ParentWithArg p = new Child2("init hoge");
	}
}

