package sample.demo.form;
import lombok.Data;
import lombok.AllArgsConstructor;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.Max;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Pattern;

import sample.demo.form.Constants;

@AllArgsConstructor
@Data
public class ConfigSystem {
	@NotBlank
	@Pattern(regexp = "^(jp|en)$", message = "Invalid format")
	private String lang;

	@NotNull
	@Min(Constants.ITEM_SORT_ID)
	@Max(Constants.ITEM_SORT_DEADLINE)
	private Integer itemSortOrder;

	@NotNull
	@Min(Constants.PROG_SORT_NEW)
	@Max(Constants.PROG_SORT_OLD)
	private Integer progressSortOrder;
}

