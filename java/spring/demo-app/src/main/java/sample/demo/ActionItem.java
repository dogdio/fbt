package sample.demo;

import sample.demo.form.*;
import sample.demo.word.*;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;


@Controller
public class ActionItem {
	private List<RegistData> itemList = new ArrayList<>();
	private Map<Integer, List<ProgressData>> progressList = new HashMap<>();
	private int idCounter = 1;

	// default: japanese
	private WordListIF wordList = new WordListJp();
	private String configLang = "jp";
	private final static RegistData nullRegistData = new RegistData(0, "", 0, 0, 0, "", "");

	public ActionItem()
	{
		//FIXME: tentative items
		itemList.add(new RegistData(idCounter++, "aaaaaaaaa", 1, 0, 2, "hoge", "2023-07-01"));
		itemList.add(new RegistData(idCounter++, "bbbbbbbbb", 5, 2, 2, "hoge", "2023-07-02"));
		itemList.add(new RegistData(idCounter++, "ccccccccc", 3, 1, 3, "hoge", "2023-07-03"));

		progressList.put(1, new ArrayList<ProgressData>());
		progressList.get(1).add(new ProgressData(1, "aaa: hoge hoge", "2023-07-03 09:25:00"));
		progressList.get(1).add(new ProgressData(2, "aaa: hoge hoge", "2023-07-04 09:26:00"));
		progressList.get(1).add(new ProgressData(3, "aaa: hoge hoge", "2023-07-05 09:27:00"));

		progressList.put(2, new ArrayList<ProgressData>());
		progressList.get(2).add(new ProgressData(1, "bbb: hoge hoge", "2023-07-07 09:35:00"));
		progressList.get(2).add(new ProgressData(2, "bbb: hoge hoge", "2023-07-08 09:36:00"));

		progressList.put(3, new ArrayList<ProgressData>());
		progressList.get(3).add(new ProgressData(1, "ccc: hoge hoge", "2023-08-07 09:35:00"));
		progressList.get(3).add(new ProgressData(2, "ccc: hoge hoge", "2023-08-08 09:36:00"));
	}

	@GetMapping("summary")
	public String summary(Model model)
	{
		model.addAttribute("lists", itemList);
		model.addAttribute("wordList", wordList);
		return "summary";
	}

	@GetMapping("new_item")
	public String newItem(Model model)
	{
		model.addAttribute("wordList", wordList);
		return "new_item";
	}

	@PostMapping("regist")
	public String regist(RegistForm arg, Model model)
	{
		System.out.println("Args| " +
			arg.getTitle() + "," + arg.getPriority() + "," + arg.getStatus() + "," +
			arg.getCategory() + "," + arg.getWorker() + "," + arg.getDeadline()
		);

		model.addAttribute("regist_number", idCounter);
		RegistData rd = new RegistData(
			idCounter++,
			arg.getTitle(), arg.getPriority(), arg.getStatus(),
			arg.getCategory(), arg.getWorker(), arg.getDeadline()
		);

		itemList.add(rd);

		model.addAttribute("wordList", wordList);
		return "regist";
	}

	@GetMapping("config")
	public String readConfig(ConfigForm arg, Model model)
	{
		model.addAttribute("wordList", wordList);

		arg.setLang(configLang);
		return "config";
	}

	// JSONを受信して、JSONを返す(Thymeleafを使わない)
	@PostMapping("writeConfig")
	@ResponseBody
	public List<ConfigForm> writeConfig(@RequestBody ConfigForm arg)
	{
		if(!configLang.equals(arg.getLang())) {
			configLang = arg.getLang();

			if(configLang.equals("jp"))
				wordList = new WordListJp();
			else if(configLang.equals("en"))
				wordList = new WordListEn();

			arg.setReload(true);
		}
		System.out.println("LANG=" + arg.getLang() + ", Reload=" + arg.getReload());

		List<ConfigForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	private RegistData GetRegistDataById(Integer itemId)
	{
		for(RegistData rd: itemList) {
			if(itemId == rd.getId()) {
				return rd;
			}
		}
		return nullRegistData;
	}

	private List<ProgressData> GetProgressListById(Integer itemId)
	{
		List<ProgressData> ret = progressList.get(itemId);
		if(ret == null) {
			progressList.put(itemId, new ArrayList<ProgressData>());
			ret = progressList.get(itemId);
		}
		return ret;
	}

	private ProgressData GetProgressDataById(List<ProgressData> pdList, Integer progressId)
	{
		for(ProgressData pd: pdList) {
			if(progressId == pd.getId()) {
				return pd;
			}
		}
		return null;
	}

	private int GenerateProgressId(List<ProgressData> pd)
	{
		if(pd.size() == 0)
			return 1;

		ProgressData tail = pd.get(pd.size() - 1);
		return tail.getId() + 1;
	}

	@GetMapping("/show/{itemId}")
	public String show(@PathVariable Integer itemId, Model model)
	{
		System.out.println("<<<<< show id=" + itemId);

		RegistData arg = GetRegistDataById(itemId);
		System.out.println("Args| " + arg.getId() + ": " +
			arg.getTitle() + "," + arg.getPriority() + "," + arg.getStatus() + "," +
			arg.getCategory() + "," + arg.getWorker() + "," + arg.getDeadline()
		);

		model.addAttribute("titleShow", "#" + arg.getId() + ", " + arg.getTitle());
		model.addAttribute("itemId", arg.getId());
		model.addAttribute("registData", arg);
		model.addAttribute("progressList", GetProgressListById(itemId));
		model.addAttribute("wordList", wordList);

		return "show";
	}

	@PostMapping("writeItem")
	@ResponseBody
	public List<RegistData> writeItem(@RequestBody RegistData arg)
	{
		System.out.println("Args| " + arg.getId() + ": " +
			arg.getTitle() + "," + arg.getPriority() + "," + arg.getStatus() + "," +
			arg.getCategory() + "," + arg.getWorker() + "," + arg.getDeadline()
		);

		RegistData dst = GetRegistDataById(arg.getId());
		dst.setTitle(arg.getTitle());
		dst.setPriority(arg.getPriority());
		dst.setStatus(arg.getStatus());
		dst.setCategory(arg.getCategory());
		dst.setWorker(arg.getWorker());
		dst.setDeadline(arg.getDeadline());

		List<RegistData> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("writeProgress")
	@ResponseBody
	public List<ProgressForm> writeProgress(@RequestBody ProgressForm arg)
	{
		System.out.println("Args| " + arg.getId() + ": " + arg.getContents());

		LocalDateTime date = LocalDateTime.now();
		DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
		String dateStr = date.format(dtf);
		System.out.println("Now| " + dateStr);

		List<ProgressData> dst = GetProgressListById(arg.getId());
		int id = GenerateProgressId(dst);
		dst.add(new ProgressData(id, arg.getContents(), dateStr));

		List<ProgressForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("/deleteProgress/{itemId}")
	@ResponseBody
	public List<ProgressForm>
	deleteProgress(@RequestBody ProgressForm arg, @PathVariable Integer itemId)
	{
		System.out.println("Args| "+ itemId + ", " + arg.getId() + ", " + arg.getContents());

		List<ProgressData> pdList = GetProgressListById(itemId);

		ProgressData del = GetProgressDataById(pdList, arg.getId());
		if(del != null)
			pdList.remove(del);

		List<ProgressForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("/updateProgress/{itemId}")
	@ResponseBody
	public List<ProgressForm>
	updateProgress(@RequestBody ProgressForm arg, @PathVariable Integer itemId)
	{
		System.out.println("Args| "+ itemId + ", " + arg.getId() + ", " + arg.getContents());

		List<ProgressData> pdList = GetProgressListById(itemId);

		ProgressData update = GetProgressDataById(pdList, arg.getId());
		if(update != null)
			update.setContents(arg.getContents());

		List<ProgressForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@GetMapping("/delete/{itemId}")
	public String delete(@PathVariable Integer itemId, Model model)
	{
		System.out.println("<<<<< delete id=" + itemId);

		RegistData del = GetRegistDataById(itemId);

		model.addAttribute("titleShow", "#" + del.getId() + ", " + del.getTitle());
		model.addAttribute("wordList", wordList);

		if(itemList.get(itemId - 1) != null) {
			itemList.remove(itemId - 1); // FIXME: buggggy

			// FIXME: delete progress
		}
		return "delete";
	}
}


