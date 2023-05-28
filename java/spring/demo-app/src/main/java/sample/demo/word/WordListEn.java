package sample.demo.word;

public class WordListEn implements WordListIF {
	public final String tableWord[] = { "ID", "Title", "Priority", "Status", "Category", "Worker", "Deadline" };
	public final String statusWord[] = { "New", "Processing", "Complete" };
	public final String categoryWord[] = {"new feature", "bug fix", "refactor", "study", "retrospective", "etc" };
	public final String linkWord[] = {"new item", "summary", "config" };

	public final String titleSummary = "Action Item Summary";
	public final String titleConfig = "Config";
	public final String titleRegist = "Registration Complete !";
	public final String titleNewItem = "New Action Item: Registration";
}
