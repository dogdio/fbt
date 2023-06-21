package sample.demo.serv;

import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;

import sample.demo.form.AccountData;
import sample.demo.form.Constants;
import sample.demo.repo.AccountRepository;

@Service
@Transactional
public class AccountServiceImpl implements AccountService {
	@Autowired
	AccountRepository repository;

	@Override
	public boolean save(AccountData data)
	{
		AccountData ad = repository.save(data);
		if(ad == null)
			return false;

		System.out.println("[Account] saved: = " + ad.getId() + ", " + ad.getRole());
		return true;
	}

	@Override
	public void deleteById(String id)
	{
		System.out.println("[Account] delete=" + id);
		repository.deleteById(id);
	}

	@Override
	public boolean isExists(String id)
	{
		boolean ret = repository.existsById(id);
		System.out.println("[Account] " + id + " isExists=" + ret);
		return ret;
	}

	@Override
	public AccountData findById(String id)
	{
		System.out.println("[Account] find id=" + id);

		Optional<AccountData> ad = repository.findById(id);
		if(ad.isPresent())
			return ad.get();
		else
			return null;
	}

	@Override
	public String encryptPassword(String pass)
	{
		BCryptPasswordEncoder encoder = getEncrypter();
		return encoder.encode(pass);
	}

	@Override
	public BCryptPasswordEncoder getEncrypter()
	{
		return new BCryptPasswordEncoder(10);
	}
}
