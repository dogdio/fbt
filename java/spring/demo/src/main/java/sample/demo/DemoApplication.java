package sample.demo;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;

import java.io.*;
import java.util.regex.*;

@SpringBootApplication
@Controller
public class DemoApplication
{
	public static void main(String[] args)
	{
		SpringApplication.run(DemoApplication.class, args);
	}

	@GetMapping("/")
	public String home(Model model)
	{
		model.addAttribute("title", "Spring Framework: Sample");
		model.addAttribute("message", "Get your syslog <a href=\"/syslog\">here</a>");
		return "home";
	}

	@GetMapping("/style1.css")
	public String style1(Model model)
	{
		return "style1.css";
	}

	public void CreateTableHeader(StringBuilder table, String filePath, int num, String argStr)
	{
		table.append("<caption><b>" + filePath + " [" + num + "], " + argStr + "</b></caption>\n");
		table.append("<tr>");
		table.append("<th>Date</th>");
		table.append("<th>Host</th>");
		table.append("<th>Process</th>");
		table.append("<th>Info</th>");
		table.append("</tr>\n");
	}

	public void CreateTableData(StringBuilder table, String cmd, String file, int num, String argStr)
	{
		Pattern p1 = Pattern.compile(argStr);
		Pattern p2 = Pattern.compile("^(.{9}:..:..)\\s([^\\s]+)\\s([^\\s]+:)\\s(.*)");

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
					table.append("<tr>");
					for(int i = 1; i < 4; i++)
						table.append("<td>" + m.group(i) + "</td>");

					String s = m.group(4);
					int len = ((s.length() > 100) ? 100 : s.length());
					table.append("<td>" + s.substring(0, len) + "</td>");
					table.append("</tr>\n");
					num--;
				}
			}
			int exitCode = proc.waitFor();
			System.out.println(cmd + " ret: " + exitCode);
		} catch (IOException e) { e.printStackTrace();
		} catch (InterruptedException e) { e.printStackTrace(); }
	}

	@GetMapping("/syslog")
	public String syslog(@RequestParam(value = "num", defaultValue = "50") String argNum, 
						@RequestParam(value = "str", defaultValue = ".") String argStr, 
						@RequestParam(value = "rev", defaultValue = "on") String argRev, 
						Model model)
	{
		String filePath = "/var/log/syslog";
		StringBuilder table = new StringBuilder();
		int num;

		System.out.println("Args: " + argNum + "," + argStr + "," + argRev + ",");
		model.addAttribute("title", "syslog search");
		model.addAttribute("message", "<a href=\"/\">top</a>");

		try { num = Integer.parseInt(argNum); }
		catch (NumberFormatException e) { num = 50; }

		table.append("<table>\n");
		CreateTableHeader(table, filePath, num, argStr);

		String cmd;
		if(argRev.equals("on"))
			cmd = "tac";
		else
			cmd = "cat";
		CreateTableData(table, cmd, filePath, num, argStr);

		table.append("</table>\n");
		model.addAttribute("message1", table.toString());

		return "syslog";
	}
}

