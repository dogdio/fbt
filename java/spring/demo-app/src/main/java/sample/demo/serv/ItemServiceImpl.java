package sample.demo.serv;

import java.util.Optional;
import java.time.LocalDate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import sample.demo.form.RegistData;
import sample.demo.form.ConfigData;
import sample.demo.repo.ItemRepository;

@Service
@Transactional
public class ItemServiceImpl implements ItemService {
	@Autowired
	ItemRepository repository;

	@Override
	public Iterable<RegistData> findAll(ConfigData config)
	{
//		return repository.findAllSortById();
		int statusMin = 0;
		int statusMax = 2;
		if(config.getStatus() != 999)
			statusMin = statusMax = config.getStatus();

		int cateMin = 0;
		int cateMax = 5;
		if(config.getCategory() != 999)
			cateMin = cateMax = config.getCategory();

		return repository.findAllWithMyQuery(
			statusMin, statusMax,
			cateMin, cateMax,
			config.getStartDate(), config.getStopDate()
		);
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

	private final static RegistData nullRegistData = new RegistData(0, "", 0, 0, 0, "", LocalDate.now());

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
