package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;
import jakarta.validation.constraints.Size;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;

@AllArgsConstructor // 全てのメンバを設定するコンストラクタを生成
@Data // getter/setterが自動生成される(getNum, setNum)
public class ProgressForm {
	@NotNull
	private Integer id;

	@NotBlank
	@Size(min = 1, max = 2000)
	private String contents;
}

