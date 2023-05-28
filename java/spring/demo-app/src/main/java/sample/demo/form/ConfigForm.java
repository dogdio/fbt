package sample.demo.form;
import lombok.Data;

@Data // getter/setterが自動生成される(getNum, setNum)
public class ConfigForm {
	private String lang;
	private String value1;
	private String value2;
}

