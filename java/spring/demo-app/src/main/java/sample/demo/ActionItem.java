package sample.demo;

import sample.demo.form.*;
import sample.demo.word.*;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.ModelAttribute;

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

@Controller
public class ActionItem {
	private List<RegistData> itemList = new ArrayList<>();
	private int idCounter = 1;

	// default: japanese
	private WordListIF wordList = new WordListJp();
	private String configLang = "jp";

	public ActionItem()
	{
		//FIXME: tentative items
		itemList.add(new RegistData(idCounter++, "aaaaaaaaa", 1, 0, 2, "hoge", "2023-07-01"));
		itemList.add(new RegistData(idCounter++, "bbbbbbbbb", 5, 2, 2, "hoge", "2023-07-02"));
		itemList.add(new RegistData(idCounter++, "ccccccccc", 3, 1, 3, "hoge", "2023-07-03"));
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

	@PostMapping("config")
	public String writeConfig(ConfigForm arg, Model model)
	{
		if(!configLang.equals(arg.getLang())) {
			configLang = arg.getLang();

			if(configLang.equals("jp"))
				wordList = new WordListJp();
			else if(configLang.equals("en"))
				wordList = new WordListEn();
		}
		model.addAttribute("wordList", wordList);

		System.out.println("LANG=" + configLang);
		return "config";
	}
}


