// package 名はディレクトリ名と同じにする
package Hoge;

public class Parent {
	private int PriNum = 1000;
	public int PubNum = 2000;
	protected int ProNum = 3000;
	int DefNum = 4000;

	public void CallPriv() {
		HogeInst.Func();
	}
}

