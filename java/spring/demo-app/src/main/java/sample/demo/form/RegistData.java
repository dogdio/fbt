package sample.demo.form;

import org.springframework.data.annotation.Id;
import lombok.Data;
import lombok.AllArgsConstructor;
//import javax.persistence.Table;
//import javax.persistence.Entity;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class RegistData {
	@Id
	private Integer id;

	private String title;
	private Integer priority;
	private Integer status;
	private Integer category;
	private String worker;
	private String deadline;
}

