import java.util.Vector;
import java.util.regex.*;

class Main {
	public static void AddInstance(HogeIF Obj) {
		System.out.println("Add: " + Obj.GetName());
		vec.add(Obj);
	}

	public static void main(String[] args) {
		AddInstance(new Hoge1());
		AddInstance(new Hoge3());
		AddInstance(new Hoge2());

		for(int i = 0; i < vec.size(); i++) {
			HogeIF Obj = vec.get(i);
			String str = Obj.GetName();
			boolean find = true;

			if(args.length == 1) {
				Pattern p = Pattern.compile(args[0]);
				Matcher m = p.matcher(str);
				find = m.find();
			}
			if(find) {
				System.out.println("<<<<< " + str + " >>>>>");
				Obj.Initialize();
				boolean ret = Obj.Exec();
				System.out.println(" Exec : " + ((ret == true) ? "OK" : "NG") ); 
				Obj.Finalize();
			}
		}
	}
	public static Vector<HogeIF> vec = new Vector<HogeIF>();
}

