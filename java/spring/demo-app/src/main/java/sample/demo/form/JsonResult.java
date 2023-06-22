package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;

@AllArgsConstructor
@Data
public class JsonResult {
	private String key;
	private String value;
	private String attr;
}

