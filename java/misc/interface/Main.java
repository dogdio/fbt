// インターフェイスクラスには関数の実装を記載しない
// 戻り値、名前、引数のみを記載する

//final interface HogeIF { // compile error: finalは記述できない
interface HogeIF {
	boolean IsOddNumber(int x);
	//void func(int x) { } // compile error: 関数を実装してはダメ
}

//class Hoge extends HogeIF { // compile error
class Hoge implements HogeIF {
	public boolean IsOddNumber(int x) {
		//if(x & 1) // compile error: bit演算の結果はbooleanではない
		if((x & 1) == 1)
			return true;
		else
			return false;
	}
	void func(int x) { }
}

class Main {
	public static void main(String[] args) {
		HogeIF h = new Hoge();
		int x = 123;
		int y = 456;

		System.out.println(x + " is Odd? " + h.IsOddNumber(x));
		System.out.println(y + " is Odd? " + h.IsOddNumber(y));

		// compile error: interfaceのインスタンスは作成できない
		//HogeIF hif = new HogeIF();
	}
}

