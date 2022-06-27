import IF.*;
import java.lang.reflect.*;
// IFパッケージには依存しているが、Sampleパッケージには依存していない
// この状態でSampleパッケージの各インスタンスのメソッドを呼ぶ

class Main {
	static public void CallStatic(String PackageName, String ClassName, String MethodName) {
		Class<?> cl = null;
		Method method = null;
		try {
			cl = Class.forName(PackageName + "." + ClassName);
		}
		catch (ClassNotFoundException e) { e.printStackTrace(); return; }

		try {
			method = cl.getDeclaredMethod(MethodName);
		}
		catch (NoSuchMethodException e) { e.printStackTrace(); return; }

		try { // PackageName.ClassName.MethodName() を呼ぶ
			method.invoke(null);
		}
		catch (IllegalAccessException e) { e.printStackTrace(); return; }
		catch (InvocationTargetException e) { e.printStackTrace(); return; }
	}
	public static void main(String[] args) {
		// Hoge,Hoge2との依存関係なしに、これらのインスタンスを生成するために
		// refrectionを使ってstatic メソッドにアクセスする
		String ClassNames[] = {"Hoge", "Hoge2"};
		for(String s: ClassNames)
			CallStatic("Sample", s, "Touch");

		// Factoryから、interfaceクラスのインスタンスを取得
		HogeIF h1 = Factory.GetInstance("x1");
		HogeIF h10 = Factory.GetInstance("x10");
		if(h1 != null)
			System.out.println("# 1 * (10 + 20)=" + h1.Add(10, 20));
		if(h10 != null)
			System.out.println("# 10 * (10 + 20)=" + h10.Add(10, 20));

		// null は文字列として出力される
		System.out.println("null po:" + Factory.GetInstance("not found"));
		System.out.println("null po:" + Factory.GetInstance(null));

		// インスタンスのリストをトラバース
		Factory.Foreach((HogeIF Obj) -> {
			System.out.println("## (100+200)" + Obj.OwnName() + "=" + Obj.Add(100, 200));
			return true;
		});
	}
}

