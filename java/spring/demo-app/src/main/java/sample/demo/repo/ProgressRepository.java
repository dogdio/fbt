package sample.demo.repo;

import org.springframework.data.jdbc.repository.query.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;
import java.util.List;

import sample.demo.form.ProgressData;

// table, field名は、abcXyz --> abc_xyz に変換される
public interface ProgressRepository extends CrudRepository<ProgressData, Integer>
{
	@Query("SELECT * FROM progress_data p WHERE p.item_id = :itemId ORDER BY progress_id asc")
	List<ProgressData> getDataAsc(@Param("itemId") int itemId);

	@Query("SELECT * FROM progress_data p WHERE p.item_id = :itemId ORDER BY progress_id desc")
	List<ProgressData> getDataDesc(@Param("itemId") int itemId);

	@Query("SELECT p.id FROM progress_data p WHERE p.item_id = :itemId")
	Iterable<Integer> getId(@Param("itemId") int itemId);

	@Query("SELECT * FROM progress_data ORDER BY id")
	Iterable<ProgressData> findAllSortById();

}
