package sample.demo.repo;

import org.springframework.data.repository.CrudRepository;
import sample.demo.form.AccountData;

public interface AccountRepository extends CrudRepository<AccountData, String>
{

}
