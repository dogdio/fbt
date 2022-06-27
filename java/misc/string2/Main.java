import java.io.UnsupportedEncodingException;

class Main {
	public static void hexdump(String str) {
		String s = "";
		int num = 0;
		for(int i = 0; i < str.length(); i++) {
			System.out.print(s.format("%02x ", (byte)str.charAt(i)));
			if(++num % 16 == 0)
				System.out.print("\n");
		}
		if(num % 16 != 0)
			System.out.print("\n");
	}
	public static void hexdump(byte[] b) {
		String s = "";
		int num = 0;
		for(byte bb: b) {
			System.out.print(s.format("%02x ", bb));
			if(++num % 16 == 0)
				System.out.print("\n");
		}
		if(num % 16 != 0)
			System.out.print("\n");
	}
	public static void main(String[] args) {
		String s = "abcdef0123456789";
		byte[] b = {};

		s += "ABCDEF";
		try {
			b = s.getBytes("UTF-8");
		}
		catch (UnsupportedEncodingException e) {  }

		hexdump(b);
		// 数値をプラスすればそのまま文字列になる
		hexdump(s + "0" + 123456789);

		if(args.length == 1)
			hexdump(args[0]);
	}
}

