import java.util.logging.Logger;

class Main {
	public static void Func() {
		StackTraceElement st = Thread.currentThread().getStackTrace()[1];

		// StackTraceしたときの行数(5)が表示される...
		LOGGER.info(String.format("%s.%s() %s:%d", 
			st.getClassName(), st.getMethodName(), st.getFileName(), st.getLineNumber()));

		StackTraceElement st2 = Thread.currentThread().getStackTrace()[2];
		LOGGER.info(String.format("%s.%s() %s:%d", 
			st2.getClassName(), st2.getMethodName(), st2.getFileName(), st2.getLineNumber()));
	}

	public static void main(String[] args) {
		String fileName = Thread.currentThread().getStackTrace()[1].getFileName();

		int lineNumber = Thread.currentThread().getStackTrace()[1].getLineNumber();
		System.out.println("File name: " + fileName);
		System.out.println("Line number: " + lineNumber);

		Func(); // st2 にはここの情報が入っている
	}
	private static final Logger LOGGER = Logger.getLogger(Main.class.getName());
}

