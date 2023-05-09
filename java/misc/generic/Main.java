import java.util.HashMap;
import java.util.Map;
import java.util.function.Function; // for lambda

class Hoge {
	Hoge(String Name) {
		this.Name = Name;
	}
	String GetName() {
		return Name;
	}
	private String Name;
}

// 型に依存しないgenericなfactory
class Factory<Obj_t> {
	public Obj_t GetInstance(String Key) {
		return map.get(Key);
	}
	public void AddInstance(String Key, Obj_t Obj) {
		System.out.println("Add: " + Key);
		map.put(Key, Obj);
	}
	public void Foreach(Function<Obj_t, Boolean> Func) {
		if(Func == null)
			return;
		for(Obj_t Obj: map.values())
			Func.apply(Obj);
	}
	// compile error: javaではできない？
	//public void Create(String Key) {
	//	map.put(Key, new Obj_t(Key));
	//}

	private Map<String, Obj_t> map = new HashMap<String, Obj_t>();
}

class Main {
	public static void main(String[] args) {
		Factory<Hoge> f = new Factory<>();
		f.AddInstance("Hoge1", new Hoge("Hoge1"));
		f.AddInstance("Hoge2", new Hoge("Hoge2"));
		f.AddInstance("Hoge3", new Hoge("Hoge3"));

		f.Foreach((Hoge Obj) -> {
			System.out.println("# " + Obj.GetName());
			return true;
		});

		System.out.println("## " + f.GetInstance("Hoge3").GetName());

		//System.out.println("## " + f.GetInstance("NotFound").GetName()); // SEGV
	}
}

