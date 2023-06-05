package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;
import java.time.LocalDate;
import org.hibernate.validator.constraints.Range;
import sample.demo.form.Constants;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class RegistForm {
	private String title;
	private Integer priority;

	@Range(min = Constants.STATUS_MIN, max = Constants.STATUS_MAX)
	private Integer status;

	private Integer category;
	private String worker;
	private LocalDate deadline;
}

