// コンパイル時は Main$1.class が生成される
enum Scream { ABESHI, HIDEBU, TAWABA, UWARABA };

class Main {
	public static void Who(Scream s) {
		String msg = s + ": screamed by ";
		switch(s) {
			case ABESHI: msg += "unknown"; break;
			case HIDEBU: msg += "Heart"; break;
			case TAWABA: msg += "Mad"; break;
			case UWARABA: msg += "Amiba"; break;
		}
		System.out.println(msg);
	}
	public static boolean Compare(Scream s1, Scream s2) {
		if(s1 == s2) {
			System.out.println(s1 + " == " + s2);
			return true;
		}
		else {
			System.out.println(s1 + " != " + s2);
			return false;
		}
	}

	public static void main(String[] args) {
		Scream s = Scream.HIDEBU;
		Scream s2 = Scream.TAWABA;
		// sに格納されているenumが文字として表示される
		System.out.println("s=" + s);

		// compile error: enumはただの数値ではないのでキャストできない
		//Scream s2 = (Scream)0;

		Compare(s, s2);
		Compare(s2, Scream.TAWABA);

		for(Scream scr: Scream.values())
			Who(scr);
	}
}

