import java.util.HashMap;
import java.util.TreeMap;
import java.util.Map;

class Main {
	// mapのvalueをmapにする
	static private Map<Integer, Integer> tmap = new TreeMap<>();
	static private Map<String, Map<Integer, Integer>> map = new HashMap<>();

	public static int GetValue(String key1, Integer key2) {
		int ret = -1;
		if(key1 == null)
			return ret;
		if(key2 == null)
			return ret;
		// retに代入するとき、Integerがintに変換される
		// nullチェックは2段階必要
		if(map.get(key1) != null)
			if(map.get(key1).get(key2) != null)
				ret = map.get(key1).get(key2);
		return ret;
	}
	public static void dump() {
		for(String key: map.keySet())
			for(Integer key2: map.get(key).keySet())
				System.out.println(key + " => {" + key2 +
					" => " + map.get(key).get(key2) + "}");
		System.out.println("");
	}
	public static void main(String[] args) {
		map.put("ABC", new TreeMap<Integer, Integer>());
		map.get("ABC").put(2000, 222);
		map.get("ABC").put(3000, 333);
		map.get("ABC").put(1000, 111);

		tmap.put(20000, 222);
		tmap.put(50000, 555);
		tmap.put(40000, 444);
		map.put("DEF", tmap);
		dump();

		Map<Integer, Integer> tmap2 = map.get("ABC");
		tmap2.put(3000, 33333); // overwrite
		map.get("DEF").put(10000, 111); // add
		dump();

		System.out.println("# DEF.10000 = " + GetValue("DEF", 10000));
		System.out.println("# XXX.10000 = " + GetValue("XXX", 10000));
		System.out.println("# nul.10000 = " + GetValue(null, 10000));
		System.out.println("# DEF.nul = " + GetValue("DEF", null));
		System.out.println("# DEF.99999 = " + GetValue("DEF", 99999));
	}
}

