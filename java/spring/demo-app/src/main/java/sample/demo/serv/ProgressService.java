package sample.demo.serv;

import java.util.List;
import sample.demo.form.ProgressData;

public interface ProgressService {

	List<ProgressData> findAll(Integer itemId, Integer sort);
	Integer save(ProgressData data);
	void deleteById(Integer id);
	void deleteAllByItemId(Integer itemId);
	boolean isExists(Integer id);
	ProgressData findById(Integer id);
	int generateProgressId(Integer itemId);
}
