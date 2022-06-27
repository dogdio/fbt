
interface HogeIF {
	int Add(int x, int y);
}

class Hoge implements HogeIF {
	public int Add(int x, int y) {
		return x + y;
	}
}
class Hoge2 implements HogeIF {
	public int Add(int x, int y) {
		return 10 * (x + y);
	}
}

class factory {
	static public HogeIF GetInstance(int n) {
		if(n == 1)
			return new Hoge();
		else if(n == 10)
			return new Hoge2();
		else
			return null;
	}
}

class Main {
	public static void main(String[] args) {
		// factoryから、interface クラスのインスタンスを取得
		HogeIF h1 = factory.GetInstance(1);
		HogeIF h10 = factory.GetInstance(10);
		System.out.println("1 * (10 + 20)=" + h1.Add(10, 20));
		System.out.println("10 * (10 + 20)=" + h10.Add(10, 20));

		// null は文字列として出力される
		System.out.println("null po:" + factory.GetInstance(123));
	}
}

