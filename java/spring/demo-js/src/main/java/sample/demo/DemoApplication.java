package sample.demo;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;

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

	// static配下に同じファイルがあった場合、static側が採用される
	@GetMapping("/home")
	public String home(Model model)
	{
		model.addAttribute("title", "Spring Framework: Sample");
		model.addAttribute("message", "Geeeeeet your syslog <a href=\"/syslog\">here</a>");
		return "home";
	}

	public List<SyslogData> CreateJson(String cmd, String file, int num, String argStr)
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

	@GetMapping("/syslog")
	public String syslog(
		@RequestParam(value = "num", defaultValue = "30") String argNum, 
		@RequestParam(value = "str", defaultValue = ".") String argStr, 
		@RequestParam(value = "rev", defaultValue = "on") String argRev, 
		Model model)
	{
		System.out.println("Args: " + argNum + "," + argStr + "," + argRev + ",");

		String filePath = "/var/log/syslog";
		model.addAttribute("title", "syslog search");
		model.addAttribute("message", "<a href=\"/\">top</a>");
		model.addAttribute("message1", filePath);

		return "syslog";
	}

	@GetMapping("/sample.json")
	@ResponseBody
	public List<SyslogData> sample_json(
		@RequestParam(value = "num", defaultValue = "30") String argNum,
		@RequestParam(value = "str", defaultValue = ".") String argStr, 
		@RequestParam(value = "rev", defaultValue = "on") String argRev)
	{
		System.out.println("Args| " + argNum + "," + argStr + "," + argRev + ",");
		String filePath = "/var/log/syslog";
		String cmd;
		int num;

		try { num = Integer.parseInt(argNum); }
		catch (NumberFormatException e) { num = 30; }

		if(argRev.equals("on"))
			cmd = "tac";
		else
			cmd = "cat";
		return CreateJson(cmd, filePath, num, argStr);
	}

	static class SyslogData {
		public SyslogData(String date, String host, String proc, String info)
		{
			this.date = date;
			this.host = host;
			this.proc = proc;
			this.info = info;
		}
		private final String date; // メンバの名前がJSONのKEYとなる(fetchSyslog 参照)
		private final String host;
		private final String proc;
		private final String info;
		// シリアライズのためにgetterが必要
		public String getDate() { return date; }
		public String getHost() { return host; }
		public String getProc() { return proc; }
		public String getInfo() { return info; }
	}
}

