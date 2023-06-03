package sample.demo.repo;

import org.springframework.data.jdbc.repository.query.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import java.time.LocalDate;

import sample.demo.form.RegistData;

public interface ItemRepository extends CrudRepository<RegistData, Integer>
{
	@Query("SELECT * FROM regist_data ORDER BY id")
	Iterable<RegistData> findAllSortById();

	@Query("SELECT * FROM regist_data r where" +
		" r.status >= :statusMin and r.status <= :statusMax and" +
		" r.category >= :cateMin and r.category <= :cateMax and" +
		" r.deadline >= :start and r.deadline <= :stop"
	)
	Iterable<RegistData> findAllWithMyQuery(
		@Param("statusMin") int statusMin,
		@Param("statusMax") int statusMax,
		@Param("cateMin") int cateMin,
		@Param("cateMax") int cateMax,
		@Param("start") LocalDate start,
		@Param("stop") LocalDate stop
	);
}
