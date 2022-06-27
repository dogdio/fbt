package IF;

import Sample.*;
// factoryがSampleに依存しているので、ポリモーフィックではあるが
// DIPはまだ実現できていない

public class factory {
	static public HogeIF GetInstance(int n) {
		if(n == 1)
			return new Hoge();
		else if(n == 10)
			return new Hoge2();
		else
			return null;
	}
}

