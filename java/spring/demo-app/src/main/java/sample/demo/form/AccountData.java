package sample.demo.form;

import org.springframework.data.annotation.Id;
import lombok.Data;
import lombok.AllArgsConstructor;

import sample.demo.form.Constants;

@AllArgsConstructor
@Data
public class AccountData {
	@Id
	private Integer id;

	private String name;
	private String pass;
	private String role;
	private Boolean enabled;

	private String lang;
	private Integer itemSortOrder;
	private Integer progressSortOrder;
}

