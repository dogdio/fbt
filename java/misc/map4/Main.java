import java.util.HashMap;
import java.util.TreeMap;
import java.util.Map;

class Hoge {
	int num;
	String str;
}

class Main {
	// mapのvalueをclassにする
	static private Map<String, Hoge> map = new HashMap<>();

	public static void dump() {
		String s = "";
		for(String key: map.keySet()) {
			String s2 = s.format("%s => {num=%d, str=%s}", key, map.get(key).num, map.get(key).str);
			System.out.println(s2);
		}
		System.out.println("");
	}
	public static void main(String[] args) {
		map.put("ABC", new Hoge());
		map.get("ABC").num = 100;
		map.get("ABC").str = "abc";

		Hoge h = new Hoge();
		h.num = 200;
		h.str = "def";
		map.put("DEF", h);
		dump();

		// valueがclassの場合はそのままメンバにアクセスする
		map.get("ABC").num = 12345;
		map.get("DEF").str += "ghi";
		dump();

		// null po
		//map.get("NotFound").str = "Exception";
	}
}

