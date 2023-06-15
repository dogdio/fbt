package sample.demo;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import java.util.List;

import sample.demo.form.AccountData;
import sample.demo.serv.AccountService;

@SpringBootTest
class DemoApplicationTests {

//	@Test
	void contextLoads() {
//		DemoApplication d = new DemoApplication();

//		StringBuilder table = new StringBuilder();
//		d.CreateTableData(table, "tac", "/var/log/syslog", 10, ".");
	}

	@Autowired private AccountService accountServ;

	@Test
	void accountTest() {
		List<UserDetails> details = accountServ.getUserDetails();

		for(UserDetails detail : details) {
			System.out.println(detail);
		}

		System.out.println(">>>> admin");
		System.out.println(accountServ.findById("admin"));

		System.out.println("\n>>>> hoge");
		System.out.println(accountServ.findById("hoge"));
	}
}
