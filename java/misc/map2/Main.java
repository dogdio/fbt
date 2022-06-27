import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;

// Mapの中にArrayがあるパターン
class Main {
	static private List<String> list = new ArrayList<>();
	static private Map<String, List<String>> map = new HashMap<>();

	public static void dump() {
		for(String key: map.keySet()) {
			String buf = "#" + map.get(key).size() + " " + key + " => {";
			for(String s: map.get(key))
				buf += s + " ";
			System.out.println(buf + "}");
		}
	}
	public static void main(String[] args) {
		// newしたArrayをputで割り当てる
		map.put("ABC", new ArrayList<String>());
		map.get("ABC").add("a");
		map.get("ABC").add("b");
		map.get("ABC").add("c");
		//map.get("NotFound").add("SEGV"); // Exception: null po

		System.out.println("list.isEmpty() = " + list.isEmpty() + ", size() = " + list.size());
		list.add("d"); list.add("e"); list.add("f");
		map.put("DEF", list);
		dump();

		System.out.println("[DEF].add(1, 'X')");
		list.add(1, "X");
		dump();

		System.out.println("[ABC].remove(0)");
		map.get("ABC").remove(0);
		dump();

		System.out.println("[DEF].set(2, Z)");
		map.get("DEF").set(2, "Z");
		dump();

		System.out.println("[DEF].contains('f') " + map.get("DEF").contains("f"));
		System.out.println("[DEF].contains('g') " + map.get("DEF").contains("g"));

		System.out.println("[ABC].clear()");
		map.get("ABC").clear();
		dump();
	}

}

