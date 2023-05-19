package sample.demo.form;
import lombok.Data; // @Data

@Data // getter/setterが自動生成される(getNum, setNum)
public class FormData {
	private Integer num;
	private String str;
	private String seq;
	private Boolean helpFlag;
	private Boolean numCheckFlag;
}

