package sample.demo.form;

import org.springframework.data.annotation.Id;
import lombok.Data;
import lombok.AllArgsConstructor;
import java.time.LocalDate;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.Size;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.FutureOrPresent;
import sample.demo.form.Constants;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class RegistData {
	@Id
	private Integer id;

	@NotBlank
	@Size(min = 1, max = 200)
	private String title;

	@NotNull
	@Min(Constants.PRIORITY_MIN)
	@Max(Constants.PRIORITY_MAX)
	private Integer priority;

	@NotNull
	@Min(Constants.STATUS_MIN+1)
	@Max(Constants.STATUS_MAX)
	private Integer status;

	@NotNull
	@Min(Constants.CATEGORY_MIN+1)
	@Max(Constants.CATEGORY_MAX)
	private Integer category;

	@NotBlank
	@Size(min = 1, max = 30)
	private String worker;

	@NotNull
	private LocalDate deadline;
}

