package sample.demo.form;

import lombok.Data;
import lombok.AllArgsConstructor;
import jakarta.validation.constraints.Size;
import jakarta.validation.constraints.NotBlank;

@AllArgsConstructor
@Data
public class ConfigPassword {

	@NotBlank
	@Size(min = 8, max = 30)
	private String newPassword1;

	@NotBlank
	@Size(min = 8, max = 30)
	private String newPassword2;
}

