import Sample.*;

class Hoge implements HogeIF {
	public void callback(){
		System.out.println("callback: " + this);
		MainCtx.Func(100);
	}
	Hoge(HogeMain ctx){
		MainCtx = ctx;
	}
	private HogeMain MainCtx;
}

class HogeMain {
	private int num = 100;
	public void Func(int n) {
		num += n;
		System.out.println("## num is " + num + "\n");
	}
	public static void MethodRef(Integer num) {
		System.out.println("## num is " + num);
	}
	public void MethodRef2(Integer num) {
		System.out.println("## num is " + num);
	}

	public void Main() {
		// ローカル変数はfinal宣言しなくてもラムダ内で参照できる
		Factory f = new Factory();
		HogeIF hif = new Hoge(this);
		HogeIF hif2 = new Hoge(this);
		Boolean ret;

		// classic style: interfaceを使ったコールバック
		f.Callback(new Hoge(this));

		// lambda: 引数=HogeIF Obj, 戻り値=bool
		ret = f.Lambda1((HogeIF Obj) -> {
			System.out.println("Lambda1:" + Obj);
			return false; // 戻り値を書かないとエラーになる
		}, hif);
		System.out.println("ret: " + ret + "\n");

		// lambda: 引数=なし, 戻り値=bool
		ret = f.Lambda2(() -> { // ローカル変数をキャプチャ可能
			System.out.println("Lambda2:" + hif2 + " num=" + num);
			//num++; // compile error: 編集できない
			return true;
		});
		System.out.println("ret: " + ret + "\n");

		// lambda: 引数=Integer Num, 戻り値=なし
		//f.Lambda3((int Num) -> {// compile error
		f.Lambda3((Integer Num) -> {
			System.out.println("Lambda3: Num is " + ++Num); // 引数は編集可能
			System.out.println("Lambda3: Num is " + ++Num);
		}, num);
		System.out.println("keep num: " + num + "\n"); // keep 200

		num = 1234;
		// static メソッド参照
		f.Lambda3(HogeMain::MethodRef, num);
		// メソッド参照
		f.Lambda3(this::MethodRef2, num);
	}
}

class Main {
	public static void main(String[] args) {
		new HogeMain().Main();
	}
}

