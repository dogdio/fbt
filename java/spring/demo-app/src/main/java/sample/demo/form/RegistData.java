package sample.demo.form;

import org.springframework.data.annotation.Id;
import lombok.Data;
import lombok.AllArgsConstructor;
import java.time.LocalDate;
import org.hibernate.validator.constraints.Range;
import org.hibernate.validator.constraints.Length;
import javax.validation.constraints.NotNull;
import javax.validation.constraints.NotBlank;
import javax.validation.constraints.FutureOrPresent;
import sample.demo.form.Constants;
//import javax.persistence.Table;
//import javax.persistence.Entity;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class RegistData {
	@Id
	private Integer id;

	@NotBlank
	@Length(min = 1, max = 200)
	private String title;

	@NotNull
	@Range(min = Constants.PRIORITY_MIN, max = Constants.PRIORITY_MAX)
	private Integer priority;

	@NotNull
	@Range(min = Constants.STATUS_MIN+1, max = Constants.STATUS_MAX)
	private Integer status;

	@NotNull
	@Range(min = Constants.CATEGORY_MIN+1, max = Constants.CATEGORY_MAX)
	private Integer category;

	@NotBlank
	@Length(min = 1, max = 30)
	private String worker;

	@NotNull
	@FutureOrPresent
	private LocalDate deadline;
}

