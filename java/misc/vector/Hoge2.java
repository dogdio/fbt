
class Hoge2 implements HogeIF {
	public void Initialize() { System.out.println(" Initialize"); }
	public void Finalize()  { System.out.println(" Finalize"); }
	public boolean Exec()  { return false; }
	public String GetName()  { return "Hoge2"; }
}

