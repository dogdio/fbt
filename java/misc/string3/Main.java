import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Main {
	public static void main(String[] args) {
		String s = "";
		String hex = "[0-9a-fA-F]+";

		if(args.length == 1)
			s = args[0];
		else
			s = "abc.def0..1234...56789";

		Pattern p = Pattern.compile(hex); // match hexadecimal
		Matcher m = p.matcher(s);

		System.out.println("str = " + s + ", reg = " + hex);
		while(m.find()) {
			System.out.println(" match hex = " + m.group());
		}

		System.out.println("hex to Z = " + m.replaceAll("Z"));
	}
}

