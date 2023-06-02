package sample.demo.form;

import org.springframework.data.annotation.Id;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class ProgressData {
	@Id
	private Integer id;

	private Integer itemId;
	private Integer progressId;
	private String contents;
	private String date;
}

