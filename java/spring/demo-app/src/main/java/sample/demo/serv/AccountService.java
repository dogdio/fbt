package sample.demo.serv;

import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.core.userdetails.UserDetails;
import java.util.List;
import sample.demo.form.AccountData;


public interface AccountService {

	boolean save(AccountData data);
	void deleteById(String id);
	boolean isExists(String id);
	AccountData findById(String id);
	String encryptPassword(String pass);
	BCryptPasswordEncoder getEncrypter();
}
