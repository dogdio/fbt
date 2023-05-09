
class Hoge1 implements HogeIF {
	public void Initialize() { System.out.println(" Initialize"); }
	public void Finalize()  { System.out.println(" Finalize"); }
	public boolean Exec()  { return true; }
	public String GetName()  { return "Hoge1"; }
}

