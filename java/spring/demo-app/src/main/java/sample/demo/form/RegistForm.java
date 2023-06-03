package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;
import java.time.LocalDate;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class RegistForm {
	private String title;
	private Integer priority;
	private Integer status;
	private Integer category;
	private String worker;
	private LocalDate deadline;
}

