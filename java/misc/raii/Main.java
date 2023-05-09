// try ブロックを抜けるときに、close() メソッドが呼び出される
// `AutoCloseable` インタフェースを実装している必要がある
class MyResource implements AutoCloseable {

	public MyResource() {
		System.out.println("# Open");
	}

	@Override
	public void close() throws Exception {
		System.out.println("# Close");
	}
}

class MyResource2 implements AutoCloseable {

	public MyResource2() {
		System.out.println("## Open");
	}

	@Override
	public void close() { // 例外無しの場合
		System.out.println("## Close");
	}
}

class Main {
	public static void main(String[] args) {
		System.out.println("try start");
		try (MyResource resource = new MyResource()) {
			// リソースを使用する処理
		} 
		catch (Exception e) {
			// 例外処理
		}
		// この時点で確実に close() される
		System.out.println("try end\n");

		System.out.println("try start");
		try (MyResource2 resource = new MyResource2()) {
			// リソースを使用する処理
		} 
		// この時点で確実に close() される
		System.out.println("try end");
	}
}

