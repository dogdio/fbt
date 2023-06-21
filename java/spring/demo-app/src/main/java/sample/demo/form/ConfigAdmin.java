package sample.demo.form;

import lombok.Data;
import lombok.AllArgsConstructor;
import jakarta.validation.constraints.Size;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;

@AllArgsConstructor
@Data
public class ConfigAdmin {

	@NotBlank
	@Size(min = 3, max = 30)
	private String username;

	@NotNull
	private Boolean passwordReset;

	@NotNull
	private Boolean enabled;

	@NotBlank
	private String action;
}

