package IF;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class Factory {
	static public HogeIF GetInstance(String Key) {
		return map.get(Key);
	}
	static public void AddInstance(String Key, HogeIF Obj) {
		System.out.println("Add: " + Key);
		map.put(Key, Obj);
	}
	static public void Foreach(Function<HogeIF, Boolean> Func) {
		if(Func == null)
			return;
		for(HogeIF Obj: map.values())
			Func.apply(Obj);
	}
	static private Map<String, HogeIF> map = new HashMap<String, HogeIF>();
}

