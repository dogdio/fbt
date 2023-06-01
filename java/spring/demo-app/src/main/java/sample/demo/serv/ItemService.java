package sample.demo.serv;

import sample.demo.form.RegistData;

public interface ItemService {

	Iterable<RegistData> findAll();
	Integer save(RegistData data);
	void deleteById(Integer id);
	boolean isExists(Integer id);
	RegistData findById(Integer id);
}
