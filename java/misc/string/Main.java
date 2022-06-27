
class Main {
	public static void main(String[] args) {
//		String s = """ // compile error: ここにコメントはかけない
		String s = """
	aaa // 先頭がずれると、、
		bbb	
		ccc
		"""; // ここにコメントはOK

		String s2 = """
			ddd // 先頭をそろえると、、
			eee	
			fff"""; // 最後の改行がなくなる

		System.out.println(s);
		System.out.println(s2);

		s = "abcdef";
		System.out.println(s + ":length=" + s.length());
		char c = s.charAt(4);
		System.out.println("s.charAt(4) c=" + c);
		System.out.println("s.charAt(0) c=" + s.charAt(0));
		//c = s; // compile error

		s = "漢字dayo";
		System.out.println(s + ":length=" + s.length());

		s = "𪘂る";// サロゲートペア f0 aa 98 82 e3 82 8b
		System.out.println(s + ":length=" + s.length());
		System.out.println(s + ":PCount=" + s.codePointCount(0, s.length()));

		s = "齧る";// 16bit e9 bd a7 e3 82 8b
		System.out.println(s + ":length=" + s.length());
		System.out.println(s + ":PCount=" + s.codePointCount(0, s.length()));

		System.out.println("isEmpty=" + s.isEmpty());
		s = "";
		System.out.println("isEmpty=" + s.isEmpty());
	}
}

