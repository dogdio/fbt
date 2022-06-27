import java.lang.ArrayIndexOutOfBoundsException;

class Main {
	public static void hexdump(byte[] b) {
		String s = "";
		String s2 = "";
		int num = 0;
		for(byte bb: b) {
			System.out.print(s.format("%02x ", bb));
			if(0x20 <= bb && bb <= 0x7E)
				s2 += s.format("%c", bb);
			else
				s2 += ".";

			num++;
			if(num % 8 == 0)
				s2 += " ";
			if(num % 16 == 0) {
				System.out.print(s2 + "\n");
				s2 = "";
			}
		}
		if(num % 16 != 0) {
			int sp = 16 - (num % 16);
			for(int i = 0; i < sp; i++)
				s2 = "   " + s2;
			System.out.print(s2 + "\n");
		}
	}

	public static void main(String[] args) {
		byte[] b = {};
		byte[] b2 = {0x12, 0x34, 0x56, 0x78}; // byte[]は固定領域
		byte[] b3;

		try { // ArrayIndexOutOfBoundsException
			b2[4] = 0x44; 
		}
		catch(ArrayIndexOutOfBoundsException e) { System.out.println("SEGV1"); }
		try { // ArrayIndexOutOfBoundsException
			b[0] = 0x11;
		}
		catch(ArrayIndexOutOfBoundsException e) { System.out.println("SEGV2"); }

		hexdump(b); // foreachは長さチェックがあるので大丈夫
		hexdump(b2);
		//hexdump(b3); // compile error: 未初期化

		// compile error: 再初期化はできない
		//b = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
		b = b2;
		hexdump(b);

		b[0] = 0x77;
		b[1] += 0xff;  // -1
		b[2] += 0x100; // 0
		b[3] += 0x101; // +1
		hexdump(b);

		//b += b2; // compile error
		//hexdump(b + b2); // compile error

		hexdump("0123456789abcdefghijklmn \t!$&/".getBytes());
	}
}

