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
import org.springframework.web.context.annotation.SessionScope;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.validation.annotation.Validated;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.validation.ObjectError;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

import java.util.ArrayList;
import java.util.List;
import java.time.LocalDateTime;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

@SessionScope
@Controller
public class ActionItem {
	// default: japanese
	private WordListIF wordList = new WordListJp();
	private ConfigData config = new ConfigData(Constants.STATUS_MIN, Constants.CATEGORY_MIN, "",
		LocalDate.now(), LocalDate.now().plusMonths(1), Constants.LANG_JP, Constants.ITEM_SORT_DEADLINE,
		"");
	private boolean isLoad = false;
	private final static String defaultPass = "password";

	@Autowired private ItemService itemServ;
	@Autowired private ProgressService progressServ;
	@Autowired private AccountService accountServ;

	public ActionItem() {
		System.out.println("<<<<< ActionItem Init");
	}

	private List<JsonResult> bindingResultToJson(BindingResult result, List<JsonResult> json)
	{
		List<ObjectError> errors = result.getAllErrors();
		for(ObjectError err : errors) {
			if(err instanceof FieldError) {
				FieldError fieldErr = (FieldError)err;
				json.add(new JsonResult(fieldErr.getField(), err.getDefaultMessage(), "ERR"));
			}
		}
		return json;
	}

	private List<JsonResult> deleteUser(ConfigAdmin arg, List<JsonResult> json)
	{
		AccountData ad = accountServ.findByName(arg.getUsername());

		if(ad == null) {
			json.add(new JsonResult("username", "not found", "ERR"));
		}
		else {
			System.out.println("<<<<< delete: " + ad.getId() + ", " + ad.getName());
			accountServ.deleteById(ad.getId());
			json.add(new JsonResult("username", "deleted", "WARN"));
		}
		return json;
	}

	private List<JsonResult> updateUser(ConfigAdmin arg, List<JsonResult> json)
	{
		AccountData ad = accountServ.findByName(arg.getUsername());

		if(ad == null) {
			json.add(new JsonResult("username", "not found", "ERR"));
		}
		else {
			boolean update = false;
			if(arg.getPasswordReset()) {
				String enc = accountServ.encryptPassword(defaultPass);
				ad.setPass(enc);
				json.add(new JsonResult("passwordReset", "updated", "INFO"));
				update = true;
			}
			if(arg.getEnabled() != ad.getEnabled()) {
				ad.setEnabled(arg.getEnabled());
				json.add(new JsonResult("enabled", "updated", "INFO"));
				update = true;
			}
			if(update)
				accountServ.save(ad);
		}
		return json;
	}

	private List<JsonResult> createUser(ConfigAdmin arg, List<JsonResult> json)
	{
		AccountData ad = accountServ.findByName(arg.getUsername());

		if(ad == null) {
			ad = new AccountData(null, arg.getUsername(), accountServ.encryptPassword(defaultPass),
					"ROLE_USER", true, "jp", 0, 0);
			json.add(new JsonResult("username", "created", "INFO"));
			accountServ.save(ad);
		}
		else {
			json.add(new JsonResult("username", "already exists", "ERR"));
		}
		return json;
	}

	private List<JsonResult> findUser(ConfigAdmin arg, List<JsonResult> json)
	{
		AccountData ad = accountServ.findByName(arg.getUsername());

		if(ad == null) {
			json.add(new JsonResult("username", "not found", "ERR"));
		}
		else {
			String s = ad.getEnabled() ? "(enabled)" : "(disabled)";
			json.add(new JsonResult("username", "found " + s, "INFO"));
		}
		return json;
	}

	private List<JsonResult> userAdminAction(ConfigAdmin arg, List<JsonResult> json)
	{
		if(arg.getAction().equals("Find"))
			return findUser(arg, json);
		else if(arg.getAction().equals("Create"))
			return createUser(arg, json);
		else if(arg.getAction().equals("Update"))
			return updateUser(arg, json);
		else if(arg.getAction().equals("Delete"))
			return deleteUser(arg, json);

		json.add(new JsonResult("action", "not found", "ERR"));
		return json;
	}

	private void saveConfig(String name)
	{
		AccountData ad = accountServ.findByName(name);
		if(ad != null) {
			ad.setLang(config.getLang());
			ad.setItemSortOrder(config.getItemSortOrder());
			ad.setPass(config.getPass());
			accountServ.save(ad);
		}
	}

	private void loadConfig(String name)
	{
		if(isLoad)
			return;

		AccountData ad = accountServ.findByName(name);
		if(ad != null) {
			config.setLang(ad.getLang());
			config.setItemSortOrder(ad.getItemSortOrder());
			config.setPass(ad.getPass());

			if(config.getLang().equals(Constants.LANG_JP))
				wordList = new WordListJp();
			else if(config.getLang().equals(Constants.LANG_EN))
				wordList = new WordListEn();

			isLoad = true;
		}
	}

	@GetMapping("login")
	public String login()
	{
		return "login";
	}

	@GetMapping("/")
	public String index(@AuthenticationPrincipal UserDetails detail)
	{
		loadConfig(detail.getUsername());
		System.out.println("[login] OK: " + detail.getUsername() + ", " + detail.getAuthorities());
		return "redirect:/summary";
	}

	@GetMapping("summary")
	public String summary(QueryForm arg, Model model,
							@AuthenticationPrincipal UserDetails detail)
	{
		loadConfig(detail.getUsername());

		System.out.println("arg " + arg.getStatus() + "," + arg.getCategory() + "," +
			arg.getWorker() + "," + arg.getStartDate() + "," + arg.getStopDate()
		);
		if(arg.getStatus() != null)
			config.setStatus(arg.getStatus());
		if(arg.getCategory() != null)
			config.setCategory(arg.getCategory());
		if(arg.getWorker() != null)
			config.setWorker(arg.getWorker());

		LocalDate date = Utils.strToLocalDate(arg.getStartDate());
		if(date != null)
			config.setStartDate(date);
		date = Utils.strToLocalDate(arg.getStopDate());
		if(date != null)
			config.setStopDate(date);

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
	public String newItem(Model model, @AuthenticationPrincipal UserDetails detail)
	{
		loadConfig(detail.getUsername());
		RegistData rf = new RegistData(-1, "new item", Constants.PRIORITY_MIN, Constants.STATUS_MIN+1,
										Constants.CATEGORY_MIN+1, "hoge", LocalDate.now());

		model.addAttribute("registData", rf);
		model.addAttribute("wordList", wordList);
		return "new_item";
	}

	@PostMapping("regist")
	public String regist(@Validated RegistData arg, BindingResult result, Model model)
	{
		System.out.println("Args| " +
			arg.getTitle() + "," + arg.getPriority() + "," + arg.getStatus() + "," +
			arg.getCategory() + "," + arg.getWorker() + "," + arg.getDeadline()
		);
		Integer id = -1;

		if (result.hasErrors()) {
			System.out.println("<<<<<<<<<<<<< Input Error");
			model.addAttribute("registData", arg);
			model.addAttribute("wordList", wordList);
			return "new_item";
		}
		else {
			arg.setId(null);
			id = itemServ.save(arg);
		}

		return "redirect:/show/" + id;
	}

	@GetMapping("config")
	public String readConfig(ConfigForm arg, Model model,
							@AuthenticationPrincipal UserDetails detail)
	{
		loadConfig(detail.getUsername());

		model.addAttribute("wordList", wordList);
		model.addAttribute("configPassword", new ConfigPassword("", ""));

		if(model.getAttribute("configAdmin") == null)
			model.addAttribute("configAdmin", new ConfigAdmin("", false, true, ""));

		if(model.getAttribute("selectedTab") == null)
			model.addAttribute("selectedTab", "tabSystem");

		arg.setLang(config.getLang());
		arg.setItemSortOrder(config.getItemSortOrder());

		return "config";
	}

	// JSONを受信して、JSONを返す(Thymeleafを使わない)
	@PostMapping("writeConfig")
	@ResponseBody
	public List<ConfigForm> writeConfig(@RequestBody ConfigForm arg,
							@AuthenticationPrincipal UserDetails detail)
	{
		loadConfig(detail.getUsername());

		if(!config.getLang().equals(arg.getLang())) {
			config.setLang(arg.getLang());

			if(config.getLang().equals(Constants.LANG_JP))
				wordList = new WordListJp();
			else if(config.getLang().equals(Constants.LANG_EN))
				wordList = new WordListEn();

			arg.setReload(true);
		}

		config.setItemSortOrder(arg.getItemSortOrder());

		saveConfig(detail.getUsername());

		System.out.println("LANG=" + arg.getLang() + ", Reload=" + arg.getReload());
		System.out.println("ItemSort=" + arg.getItemSortOrder());

		List<ConfigForm> ret = new ArrayList<>();
		ret.add(arg);
		return ret;
	}

	@PostMapping("writeConfigPassword")
	public String writeConfigPassword(@Validated ConfigPassword arg, BindingResult result,
		Model model, RedirectAttributes attr,
		@AuthenticationPrincipal UserDetails detail)
	{
		loadConfig(detail.getUsername());

		System.out.println("pass1=" + arg.getNewPassword1());
		System.out.println("pass2=" + arg.getNewPassword2());

		model.addAttribute("wordList", wordList);
		model.addAttribute("configPassword", arg);
		model.addAttribute("configForm", new ConfigForm(
			config.getLang(), config.getItemSortOrder(), "", false
		));
		model.addAttribute("configAdmin", new ConfigAdmin("", false, true, ""));

		if(!arg.getNewPassword1().equals(arg.getNewPassword2())) {
			FieldError fieldError = new FieldError("arg", "newPassword2", "unmatch password");
			result.addError(fieldError);
		}
		if (result.hasErrors()) {
			System.out.println("<<<<<<<<<<<<< Input Error");
			model.addAttribute("selectedTab", "tabPassword");
			return "config";
		}
		
		String enc = accountServ.encryptPassword(arg.getNewPassword1());
		System.out.println(enc);
		config.setPass(enc);
		saveConfig(detail.getUsername());

		attr.addFlashAttribute("selectedTab", "tabPassword");
		return "redirect:/config";
	}

	@PostMapping("/admin/userAdmin")
	@ResponseBody
	public List<JsonResult> adminUserAdmin(@RequestBody @Validated ConfigAdmin arg, 
		BindingResult result, @AuthenticationPrincipal UserDetails detail)
	{
		List<JsonResult> ret = new ArrayList<>();

		loadConfig(detail.getUsername());
		System.out.println("Name   : " + arg.getUsername());
		System.out.println("Reset  : " + arg.getPasswordReset());
		System.out.println("Enabled: " + arg.getEnabled());
		System.out.println("Action : " + arg.getAction());

		if(result.hasErrors())
			return bindingResultToJson(result, ret);

		return userAdminAction(arg, ret);
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
			return "redirect:/summary";
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

		return "redirect:/summary";
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


