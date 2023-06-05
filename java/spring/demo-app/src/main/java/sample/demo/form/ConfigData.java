package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;
import java.time.LocalDate;

@AllArgsConstructor
@Data
public class ConfigData {
	private Integer status;
	private Integer category;
	private String worker;
	private LocalDate startDate;
	private LocalDate stopDate;

	private String lang;
	private Integer itemSortOrder;
}

