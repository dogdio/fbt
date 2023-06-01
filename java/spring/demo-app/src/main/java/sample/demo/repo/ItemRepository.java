package sample.demo.repo;

import org.springframework.data.jdbc.repository.query.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import sample.demo.form.RegistData;

public interface ItemRepository extends CrudRepository<RegistData, Integer>
{
	@Query("SELECT * FROM regist_data ORDER BY id")
	Iterable<RegistData> findAllSortById();
}
