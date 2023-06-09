package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor
@Data
public class QueryForm {
	private Integer status;
	private Integer category;
	private String worker;
	private String startDate;
	private String stopDate;
}

