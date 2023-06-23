package sample.demo.form;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;

public class Utils {

	public static LocalDate strToLocalDate(String str)
	{
		LocalDate date = null;
		if(str != null && str.length() != 0) {
			try {
				date = LocalDate.parse(str, DateTimeFormatter.ISO_DATE);
			}
			catch (DateTimeParseException e) {

			}
		}
		return date;
	}

	public static String getCurrentDate()
	{
		LocalDateTime date = LocalDateTime.now();
		DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
		return date.format(dtf);
	}
}
