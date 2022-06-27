import java.util.HashMap;
import java.util.Map;

enum Scream { ABESHI, HIDEBU, TAWABA, UWARABA };

class Main {
	static private Map<String, Scream> map = new HashMap<>();

	public static void main(String[] args) {
		System.out.println("isEmpty() = " + map.isEmpty() + ", size() = " + map.size());
		map.put("Heart", Scream.HIDEBU);
		map.put("Amiba", Scream.UWARABA);
		map.put("Mad", Scream.TAWABA);
		System.out.println("isEmpty() = " + map.isEmpty() + ", size() = " + map.size());

		for(Scream s: map.values())
			System.out.println("# " + s);
		for(String key: map.keySet())
			System.out.println("## " + key + " -> " + map.get(key));

		// keyに対応するenum(class)が存在しない場合はnullが返る
		System.out.println("get(Hoge) = " + map.get("Hoge"));

		// 削除された要素が返る(見つからないときはnull)
		System.out.println("remove(Hoge)" + map.remove("Hoge"));
		System.out.println("remove(Amiba)" + map.remove("Amiba"));
		System.out.println("isEmpty() = " + map.isEmpty() + ", size() = " + map.size());

		for(String key: map.keySet())
			System.out.println("## " + key + " -> " + map.get(key));

		System.out.println("clear()"); map.clear();
		System.out.println("isEmpty() = " + map.isEmpty() + ", size() = " + map.size());
	}

}

