package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor
@Data // getter/setterが自動生成される(getNum, setNum)
public class ConfigForm {
	private String lang;
	private Integer itemSortOrder;
	private String value2;
	private Boolean reload;
}

