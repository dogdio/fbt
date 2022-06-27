package Sample;
import java.util.function.*;

public class Factory {

	public void Callback(HogeIF hoge) {
		hoge.callback();
	}

	public Boolean Lambda1(Function<HogeIF, Boolean> Func, HogeIF hoge) {
		if(Func == null)
			return false;
		else
			return Func.apply(hoge);
	}

	public Boolean Lambda2(BooleanSupplier Func) {
		if(Func == null)
			return false;
		else
			return Func.getAsBoolean();
	}

	//public void Lambda3(Consumer<int> Func, int num) { // compile error
	public void Lambda3(Consumer<Integer> Func, int num) {
		if(Func == null)
			return;
		else
			Func.accept(num);
	}
}

