package sample.demo;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.util.matcher.RequestMatcher;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.ObjectPostProcessor;
import org.springframework.security.authentication.AuthenticationManager;
import javax.sql.DataSource;

import sample.demo.serv.AccountService;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

	@Autowired private AccountService accountServ;
	@Autowired private DataSource dataSource;
	@Autowired private ObjectPostProcessor<Object> objectPostProcessor;

	@Bean
	public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception
	{
		// pub配下は認証なしで許可
		// admin配下はADMINロールのユーザーのみ許可
		// 上記以外は認証が必要
		http.authorizeRequests().requestMatchers("/pub/**").permitAll()
								.requestMatchers("/admin/**").hasRole("ADMIN")
								.anyRequest().authenticated();

		// カスタムURL=/loginとして全てのユーザーからのアクセスを認める
		// logout画面に全てのユーザーからのアクセスを認める
		http.formLogin().loginPage("/login").permitAll()
			.and()
			.logout().permitAll(); // これがないとlogout直後にまたリダイレクトされる

		return http.build();
	}

	@Bean
	public AuthenticationManager authManager() throws Exception
	{
		AuthenticationManagerBuilder auth = new AuthenticationManagerBuilder(objectPostProcessor);

		auth.jdbcAuthentication()
			.dataSource(dataSource)
			.usersByUsernameQuery("SELECT id, pass, enabled FROM account_data WHERE id = ?")
			.authoritiesByUsernameQuery("SELECT id, role FROM account_data WHERE id = ?")
			.passwordEncoder(accountServ.getEncrypter());

		return auth.build();
	}
}
