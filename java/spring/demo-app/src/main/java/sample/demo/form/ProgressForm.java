package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class ProgressForm {
	private Integer id;
	private String contents;
}

