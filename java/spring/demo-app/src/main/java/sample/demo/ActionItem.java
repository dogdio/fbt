package sample.demo;

import sample.demo.form.*;
import sample.demo.word.*;
import sample.demo.serv.*;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.validation.annotation.Validated;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;

import java.util.ArrayList;
import java.util.List;
import java.time.LocalDateTime;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;

@Controller
public class ActionItem {
	// default: japanese
	private WordListIF wordList = new WordListJp();
	private ConfigData config = new ConfigData(Constants.STATUS_MIN, Constants.CATEGORY_MIN, "",
		LocalDate.now(), LocalDate.now().plusMonths(1), Constants.LANG_JP, Constants.ITEM_SORT_DEADLINE);

	@Autowired ItemService itemServ;
	@Autowired ProgressService progressServ;

	@GetMapping("summary")
	public String summary(ConfigData arg, Model model)
	{
		System.out.println("arg " + arg.getStatus() + "," + arg.getCategory() + "," +
			arg.getWorker() + "," + arg.getStartDate() + "," + arg.getStopDate()
		);
		if(arg.getStatus() != null)
			config.setStatus(arg.getStatus());
		if(arg.getCategory() != null)
			config.setCategory(arg.getCategory());
		if(arg.getWorker() != null)
			config.setWorker(arg.getWorker());
		if(arg.getStartDate() != null)
			config.setStartDate(arg.getStartDate());
		if(arg.getStopDate() != null)
			config.setStopDate(arg.getStopDate());

		Iterable<RegistData> rdList  = itemServ.findAll(config);

		List<RegistData> rdList2 = new ArrayList<RegistData>();
		for(RegistData rd : rdList) {
			if(config.getWorker().length() != 0) {
				if(!config.getWorker().equals(rd.getWorker())) {
					continue;
				}
			}

			rdList2.add(rd);
			System.out.println("RD " + rd.getId() + ":" +
				rd.getTitle() + "," + rd.getPriority() + "," + rd.getStatus() + "," +
				rd.getCategory() + "," + rd.getWorker() + "," + rd.getDeadline()
			);
		}
		model.addAttribute("lists", rdList2);
		model.addAttribute("configData", config);
		model.addAttribute("wordList", wordList);
		return "summary";
	}

	@GetMapping("new_item")
	public String newItem(Model model)
	{
		RegistForm rf = new RegistForm(-1, "new item", Constants.PRIORITY_MIN, Constants.STATUS_MIN+1,
										Constants.CATEGORY_MIN+1, "hoge", "");

		model.addAttribute("registForm", rf);
		model.addAttribute("wordList", wordList);
		return "new_item";
	}

	@PostMapping("regist")
	public String regist(@Validated RegistForm arg, BindingResult result, Model model)
	{
		System.out.println("Args| " +
			arg.getTitle() + "," + arg.getPriority() + "," + arg.getStatus() + "," +
			arg.getCategory() + "," + arg.getWorker() + "," + arg.getDeadline()
		);
		Integer id = -1;
		LocalDate deadline = null;

		if(arg.getDeadline() != null && arg.getDeadline().length() != 0) {
			try {
				deadline = LocalDate.parse(arg.getDeadline(), DateTimeFormatter.ISO_DATE);
			}
			catch (DateTimeParseException e){
				FieldError fieldError = new FieldError("arg", "deadline", "must be: yyyy-mm-dd");
				result.addError(fieldError);
			}
		}

		if (result.hasErrors()) {
			System.out.println("<<<<<<<<<<<<< Input Error");
			model.addAttribute("registForm", arg);
			model.addAttribute("wordList", wordList);
			return "new_item";
		}
		else {
			RegistData rd = new RegistData(null, arg.getTitle(), arg.getPriority(), arg.getStatus(),
											arg.getCategory(), arg.getWorker(), deadline);
			System.out.println("Date " + deadline);
			id = itemServ.save(rd);
		}

		// HTML内のinline Javascriptにidを埋め込む
		model.addAttribute("destURL", "/show/" + id);
		return "transition";
	}

	@GetMapping("config")
	public String readConfig(ConfigForm arg, Model model)
	{
		model.addAttribute("wordList", wordList);

		arg.setLang(config.getLang());
		arg.setItemSortOrder(config.getItemSortOrder());

		return "config";
	}

	// JSONを受信して、JSONを返す(Thymeleafを使わない)
	@PostMapping("writeConfig")
	@ResponseBody
	public List<ConfigForm> writeConfig(@RequestBody ConfigForm arg)
	{
		if(!config.getLang().equals(arg.getLang())) {
			config.setLang(arg.getLang());

			if(config.getLang().equals(Constants.LANG_JP))
				wordList = new WordListJp();
			else if(config.getLang().equals(Constants.LANG_EN))
				wordList = new WordListEn();

			arg.setReload(true);
		}

		config.setItemSortOrder(arg.getItemSortOrder());

		System.out.println("LANG=" + arg.getLang() + ", Reload=" + arg.getReload());
		System.out.println("ItemSort=" + arg.getItemSortOrder());

		List<ConfigForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@GetMapping("/show/{itemId}")
	public String show(@PathVariable Integer itemId, Model model)
	{
		RegistData rd = itemServ.findById(itemId);

		System.out.println("[show] " + rd.getId() + ": " +
			rd.getTitle() + "," + rd.getPriority() + "," + rd.getStatus() + "," +
			rd.getCategory() + "," + rd.getWorker() + "," + rd.getDeadline()
		);
		if(rd.getId() == 0) {
			model.addAttribute("destURL", "/summary");
			return "transition";
		}

		model.addAttribute("titleShow", "#" + rd.getId() + ", " + rd.getTitle());
		model.addAttribute("itemId", rd.getId());
		model.addAttribute("registData", rd);
		model.addAttribute("progressList", progressServ.findAll(itemId));
		model.addAttribute("wordList", wordList);

		return "show";
	}

	@PostMapping("writeItem")
	@ResponseBody
	public List<RegistData> writeItem(@RequestBody RegistData arg)
	{
		System.out.println("[writeItem] " + arg.getId() + ": " +
			arg.getTitle() + "," + arg.getPriority() + "," + arg.getStatus() + "," +
			arg.getCategory() + "," + arg.getWorker() + "," + arg.getDeadline()
		);

		if(itemServ.isExists(arg.getId())) {
			RegistData rd = new RegistData(
				arg.getId(), arg.getTitle(), arg.getPriority(), arg.getStatus(),
				arg.getCategory(), arg.getWorker(), arg.getDeadline()
			);
			itemServ.save(rd);
		}

		List<RegistData> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("writeProgress")
	@ResponseBody
	public List<ProgressForm> writeProgress(@RequestBody ProgressForm arg)
	{
		System.out.println("Args| " + arg.getId() + ": " + arg.getContents());

		if(itemServ.isExists(arg.getId())) {
			LocalDateTime date = LocalDateTime.now();
			DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
			String dateStr = date.format(dtf);
			System.out.println("Now| " + dateStr);

			int progressId = progressServ.generateProgressId(arg.getId());
			ProgressData pd = new ProgressData(null, arg.getId(), progressId,
												arg.getContents(), dateStr);
			progressServ.save(pd);
		}

		List<ProgressForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("/deleteProgress/{itemId}")
	@ResponseBody
	public List<ProgressForm>
	deleteProgress(@RequestBody ProgressForm arg, @PathVariable Integer itemId)
	{
		System.out.println("### Delete key: " + arg.getId() +
			", item=" + itemId + ", " + arg.getContents());

		if(progressServ.isExists(arg.getId()))
			progressServ.deleteById(arg.getId());

		List<ProgressForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("/updateProgress/{itemId}")
	@ResponseBody
	public List<ProgressForm>
	updateProgress(@RequestBody ProgressForm arg, @PathVariable Integer itemId)
	{
		System.out.println("### Update key: " + arg.getId() +
			", item=" + itemId + ", " + arg.getContents());

		ProgressData pd = progressServ.findById(arg.getId());
		if(pd.getId() == arg.getId()) {
			pd.setContents(arg.getContents());
			progressServ.save(pd);
		}

		List<ProgressForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@GetMapping("/delete/{itemId}")
	public String delete(@PathVariable Integer itemId, Model model)
	{
		if(itemServ.isExists(itemId)) {
			itemServ.deleteById(itemId);
			progressServ.deleteAllByItemId(itemId);
		}

		model.addAttribute("destURL", "/summary");
		return "transition";
	}

	@GetMapping("/dump/{itemId}")
	@ResponseBody
	public String dump(@PathVariable Integer itemId)
	{
//		int ret = progressServ.generateProgressId(itemId);
//		System.out.println("ret = " + ret);
//		progressServ.deleteAllByItemId(itemId);
		return "OK";
	}
}


