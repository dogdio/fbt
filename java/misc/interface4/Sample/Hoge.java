package Sample;
import IF.*;

// classをpublicにしておかないと外から呼べない
public class Hoge implements HogeIF {
	public int Add(int x, int y) {
		return x + y;
	}
	public String OwnName() {
		return "x1";
	}
	// コンストラクタはpublicである必要はない
	private Hoge() {
		// 自分で自分をFactoryにAddする(FactoryはSampleをimportしない)
		Factory.AddInstance("x1", this);
	}

	// Instanceの作成/削除が必要なら、Create/Destroy にする
	// 一度作ってそれっきりなら、Touch で十分
	static public void Touch() {
		System.out.println("Hoge Touch");
	}
	static private Hoge Inst = new Hoge();
	// static関数が呼ばれると、static変数も確保される
	// static変数を自動で生成する方法が不明(外からきっかけが必要)
}

