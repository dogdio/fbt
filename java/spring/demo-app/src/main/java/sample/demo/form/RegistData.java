package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class RegistData {
	private Integer id;
	private String title;
	private Integer priority;
	private Integer status;
	private Integer category;
	private String worker;
	private String deadline;
}

