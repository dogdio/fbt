package sample.demo.serv;

import java.util.Optional;
import java.time.LocalDate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import sample.demo.form.RegistData;
import sample.demo.form.ConfigData;
import sample.demo.form.Constants;
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
		int statusMin = Constants.STATUS_MIN + 1;
		int statusMax = Constants.STATUS_MAX;
		if(config.getStatus() != Constants.STATUS_MIN)
			statusMin = statusMax = config.getStatus();

		int cateMin = Constants.CATEGORY_MIN + 1;
		int cateMax = Constants.CATEGORY_MAX;
		if(config.getCategory() != Constants.CATEGORY_MIN)
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
