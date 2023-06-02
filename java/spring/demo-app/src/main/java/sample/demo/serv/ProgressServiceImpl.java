package sample.demo.serv;

import java.util.Optional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import java.util.List;

import sample.demo.form.ProgressData;
import sample.demo.repo.ProgressRepository;

@Service
@Transactional
public class ProgressServiceImpl implements ProgressService {
	@Autowired
	ProgressRepository repository;

	@Override
	public List<ProgressData> findAll(Integer itemId) // + FIXME: reverse
	{
		return repository.getData(itemId);
	}

	@Override
	public Integer save(ProgressData data)
	{
		ProgressData rd = repository.save(data);
		System.out.println("[Progress] saved: = " + rd.getItemId() + ", " + rd.getProgressId());
		return rd.getId();
	}

	@Override
	public void deleteById(Integer id)
	{
		System.out.println("[Progress] delete=" + id);
		repository.deleteById(id);
	}

	@Override
	public void deleteAllByItemId(Integer itemId)
	{
		System.out.println("[Progress] delete all=" + itemId);

		Iterable<Integer> ids = repository.getId(itemId);
		repository.deleteAllById(ids);
	}

	@Override
	public boolean isExists(Integer id)
	{
		boolean ret = repository.existsById(id);
		System.out.println("[Progress] " + id + " isExists=" + ret);
		return ret;
	}

	private final static ProgressData nullProgressData = new ProgressData(0, 0, 0, "", "");

	@Override
	public ProgressData findById(Integer id)
	{
		System.out.println("[Progress] find id=" + id);
		Optional<ProgressData> rd = repository.findById(id);
		if(rd.isPresent())
			return rd.get();
		else
			return nullProgressData;
	}

	@Override
	public int generateProgressId(Integer itemId)
	{
		List<ProgressData> pdList = repository.getData(itemId);

		if(pdList.size() == 0)
			return 1;

		ProgressData tail = pdList.get(pdList.size() - 1);
		return tail.getProgressId() + 1;
	}
}



