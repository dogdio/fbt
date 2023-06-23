package sample.demo.form;
import java.time.LocalDate;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor
@Data
public class QueryForm {
	private Integer status;
	private Integer category;
	private String worker;
	private LocalDate startDate;
	private LocalDate stopDate;
}

