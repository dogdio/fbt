package sample.demo;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;

import sample.demo.form.*;

import java.io.*;
import java.util.regex.*;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;

@SpringBootApplication
@Controller
public class DemoApplication
{
	public static void main(String[] args)
	{
		SpringApplication.run(DemoApplication.class, args);
	}

	@GetMapping("/home")
	public String home(Model model)
	{
		model.addAttribute("title", "Spring Framework: Sample");
		// th:utext の場合はタグをそのまま埋め込める
		model.addAttribute("message", "Get youuuuuuuuur syslog <a href=\"/syslog\">here</a>");
		return "home";
	}

	public List<SyslogData> CreateSyslogData(String cmd, String file, int num, String argStr)
	{
		Pattern p1 = Pattern.compile(argStr);
		Pattern p2 = Pattern.compile("^(.{9}:..:..)\\s([^\\s]+)\\s([^\\s]+:)\\s(.*)");
		List<SyslogData> list = new ArrayList<>();

		try {
			Process proc = new ProcessBuilder(cmd, file).start();
			BufferedReader br = new BufferedReader(new InputStreamReader(proc.getInputStream()));
			String line;
			int lnum = 0;
			while ((line = br.readLine()) != null) {
				if(num <= 0)
					continue;

				Matcher m = p1.matcher(line);
				if(!m.find())
					continue;

				m = p2.matcher(line);
				if(m.find()) {
					String s = m.group(4);
					int len = ((s.length() > 90) ? 90 : s.length());

					list.add( new SyslogData(m.group(1), m.group(2), m.group(3), s.substring(0, len)) );
					num--;
					//System.out.println("num: " + num);
				}
			}
			int exitCode = proc.waitFor();
			System.out.println(cmd + " ret: " + exitCode);
		} catch (IOException e) { e.printStackTrace();
		} catch (InterruptedException e) { e.printStackTrace(); }

		return list;
	}

	// 引数をFormData型にすればModelに紐付けられる(addAttribute不要)
	// htmlでは、th:object="${formData}"とすればOK
	// FormDataの先頭(F)を小文字にしないとダメ...
	@GetMapping("/syslog")
	public String syslog(FormData arg, Model model)
	{
		// formに入力された値はargに格納される
		// argを th:field で展開すれば、ブラウザには入力値がそのまま再表示される
		// <select>については、th:selected で条件成立したものだけを表示
		System.out.println("Args| " + arg.getNum() + "," + arg.getStr() + "," + 
			arg.getSeq() + "," + arg.getHelpFlag() + "," + arg.getNumCheckFlag());

		String filePath = "/var/log/syslog";
		String cmd = "tac";
		String str = ".";
		int num = 30;

		model.addAttribute("title", "syslog search");
		model.addAttribute("message", "<a href=\"/home\">top</a>");
		model.addAttribute("message1", filePath);

		// @RequestParam を使わない場合は、nullチェック必要
		if(arg.getNum() != null)
			num = arg.getNum();

		if(arg.getSeq() != null) {
			if(arg.getSeq().equals("old"))
				cmd = "cat";
		}
		if(arg.getStr() != null) {
			if(!arg.getStr().equals(""))
				str = arg.getStr();
		}

		// htmlでは th:each="list : ${lists}" でListを展開可能
		// 各要素には [[${list.date}]] でアクセス可能
		List<SyslogData> data = CreateSyslogData(cmd, filePath, num, str);
		model.addAttribute("lists", data);

		return "syslog";
	}

	class SyslogData {
		public SyslogData(String date, String host, String proc, String info)
		{
			this.date = date;
			this.host = host;
			this.proc = proc;
			this.info = info;
		}
		public final String date;
		public final String host;
		public final String proc;
		public final String info;
	}
}

