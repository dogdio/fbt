import IF.*;

class Main {
	public static void main(String[] args) {
		// factoryから、interfaceクラスのインスタンスを取得
		HogeIF h1 = factory.GetInstance(1);
		HogeIF h10 = factory.GetInstance(10);
		System.out.println("1 * (10 + 20)=" + h1.Add(10, 20));
		System.out.println("10 * (10 + 20)=" + h10.Add(10, 20));

		// null は文字列として出力される
		System.out.println("null po:" + factory.GetInstance(123));
	}
}

