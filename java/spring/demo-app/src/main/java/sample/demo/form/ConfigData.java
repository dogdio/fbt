package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;
import java.time.LocalDate;

@AllArgsConstructor
@Data
public class ConfigData {
	Integer status;
	Integer category;
	String worker;
	LocalDate startDate;
	LocalDate stopDate;
}

