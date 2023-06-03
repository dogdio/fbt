package sample.demo.serv;

import sample.demo.form.RegistData;
import sample.demo.form.ConfigData;

public interface ItemService {

	Iterable<RegistData> findAll(ConfigData config);
	Integer save(RegistData data);
	void deleteById(Integer id);
	boolean isExists(Integer id);
	RegistData findById(Integer id);
}
