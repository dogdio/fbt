package sample.demo.word;

public class WordListJp implements WordListIF {
	public final String tableWord[] = { "識別子", "タイトル", "優先度", "状態", "種類", "作業者", "期限" };
	public final String statusWord[] = { "未着手", "作業中", "完了" };
	public final String categoryWord[] = {"新機能作成", "バグ対応", "リファクタリング", "教育/学習", "分析/振り返り", "その他" };
	public final String linkWord[] = {"新規登録", "一覧", "設定" };

	public final String titleSummary = "アクションアイテム 一覧";
	public final String titleConfig = "設定";
	public final String titleRegist = "登録完了！";
	public final String titleNewItem = "新規アクションアイテム 登録";
	public final String titleDelete = "以下のアイテムは削除されました";
}
