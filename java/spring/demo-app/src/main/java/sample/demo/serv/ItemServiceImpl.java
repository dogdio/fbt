package sample.demo.serv;

import java.util.Optional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import sample.demo.form.RegistData;
import sample.demo.repo.ItemRepository;

@Service
@Transactional
public class ItemServiceImpl implements ItemService {
	@Autowired
	ItemRepository repository;

	@Override
	public Iterable<RegistData> findAll()
	{
		return repository.findAllSortById();
	}

	@Override
	public Integer save(RegistData data)
	{
		RegistData rd = repository.save(data);
		System.out.println("[Item] saved: = " + rd.getId() + ", " + rd.getTitle());
		return rd.getId();
	}

	@Override
	public void deleteById(Integer id)
	{
		System.out.println("[Item] delete=" + id);
		repository.deleteById(id);
	}

	@Override
	public boolean isExists(Integer id)
	{
		boolean ret = repository.existsById(id);
		System.out.println("[Item] " + id + " isExists=" + ret);
		return ret;
	}

	private final static RegistData nullRegistData = new RegistData(0, "", 0, 0, 0, "", "");

	@Override
	public RegistData findById(Integer id)
	{
		System.out.println("[Item] find id=" + id);
		Optional<RegistData> rd = repository.findById(id);
		if(rd.isPresent())
			return rd.get();
		else
			return nullRegistData;
	}
}
