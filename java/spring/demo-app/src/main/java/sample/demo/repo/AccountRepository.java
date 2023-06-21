package sample.demo.repo;

import org.springframework.data.repository.CrudRepository;
import org.springframework.data.jdbc.repository.query.Query;
import org.springframework.data.repository.query.Param;
import sample.demo.form.AccountData;

public interface AccountRepository extends CrudRepository<AccountData, Integer>
{
	@Query("SELECT * FROM account_data a WHERE a.name = :username")
	AccountData findByName(@Param("username") String username);
}
