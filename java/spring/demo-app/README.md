
## demo-app 概要

### 設定ファイル
- ./src/main/resources/application.properties
  - DBの設定などを記載する
- build.gradle
  - 必要なモジュール、Javaのバージョンなどを定義しておく

## Thymeleaf
demo-app で使用しているものを中心に記述。マニュアルは以下
- https://www.thymeleaf.org/doc/tutorials/3.0/usingthymeleaf_ja.html
- https://www.thymeleaf.org/doc/articles/springsecurity.html

|パス                          |HTMLファイル                 |
|------------------------------|-----------------------------|
|src/main/resources/templates/ | テンプレート機能を使用する  |
|src/main/resources/static/    | テンプレート機能を使用しない|

templates配下のhtmlをローカルで開くと、変数の部分は展開されない（枠だけが表示される感じ）。

### ハンドラとURLの紐づけ

アプリ側は`@GetMapping("xxx")`, `@PostMapping("yyy")` などでURLとハンドラを紐づけしておく。
staticなhtmlはファイルをそのまま返すだけなので、ハンドラは不要。

```java
@GetMapping("config") // http://hoge.com/config にGETが来た時のハンドラ
public String readConfig(ConfigSystem arg, Model model,
                         @AuthenticationPrincipal UserDetails detail)
{
         //
         // config.html のテンプレートを操作する (略)
         //
    return "config"; // templates/config.html を返す
}
```

### th:object

model.addAttribute()でオブジェクトや文字列をhtmlの目印 `th:***` と紐づける
```java
// 第1引数の文字列が、th:objectの右辺となる
model.addAttribute("wordList", wordList);

// wordListは WordListIF型の オブジェクト
private WordListIF wordList = new WordListJp();
```

${wordList}とオブジェクトを紐づけし、`[[${wordList.titleConfig}]]` でメンバ変数の文字列を展開する。
```html
<div><h1 th:object="${wordList}">[[${wordList.titleConfig}]]</h1></div>
```

### th:value
メンバ変数や、通常の変数を展開する。`value="***"` のように展開される。
```html
<td><input name="username" id="username" class="customInput1" th:value="${configAdmin.username}" ></td>
```

### th:field
`th:object` で取得したオブジェクトのメンバ変数を展開する。`value="***"` のように展開される。
`id=` がない場合は `id="メンバ名"` も展開される。

```html
<td><input name="worker" id="worker" maxlength="35" th:field="${registData.worker}" class="customInput1"></td>
```

### th:replace
`header.html の "UserInfo"` というフラグメントに置き換える。もともと記載のあった部分(th:replace ～) はタグごと消える。

```html
<div style="display: flex;">
  <div th:replace="~{header :: UserInfo }"></div>
</div>
```

★置き換え後
```html
<div style="display: flex;">
  ***** ここに展開
</div>
```

フラグメント側（replaceする内容は、`th:fragment` とする）<br>
置き換え後は、`<div style="position: absolute; right: 10px; .... ` のように th:fragmentの部分は消えて、
後はそのまま表示される。

```html
<div th:fragment="UserInfo"
     style="position: absolute; right: 10px; display: flex; padding-top: 15px;">
  <div style="padding: 4px 8px 0 0;">
             :
             : 略 (ここでもテンプレートは使える)
</div>
```

### th:insert
`footer.html の "links"` というフラグメントを挿入する。もともと記載のあったタグの「中」に挿入される。

```html
<div th:insert="~{footer :: links}"></div>
```

★置き換え後
```html
<div> ***** ここに挿入 </div>
```

### th:selected
`configSystem.lang` の値が "jp" の場合は、`Japanese` が選択肢の初期値となる。"en" の場合は、`English`。
選ばれた方に、`selected="selected"` が付与される

```html
<select name="lang" id="lang" class="customInput1">
  <option value="jp" th:selected="${configSystem.lang == 'jp'}">Japanese</option>
  <option value="en" th:selected="${configSystem.lang == 'en'}">English</option>
</select>
```

### th:if
条件が成立したときのみ、表示される。
`${param}` でリクエストパラメータにアクセスできる。`http://hoge.com/login?error` の場合は上が採用される
```html
<div th:if="${param.error}">
  Invalid username and password.</div>
<div th:if="${param.logout}">
  You have been logged out.</div>
```


### th:each
ループの対象となるのが `${lists}` で、各要素を `list` に展開する。`th:for` よりも `th:each` のほうが新しい(Thymeleaf 3.X以降)。

```java
List<RegistData> rdList2 = new ArrayList<RegistData>();
model.addAttribute("lists", rdList2); // rdList2.add()でN個 登録(N回 th:each を繰り返し)
```

```html
<tr class="itemTableBody" th:each="list : ${lists}" id="itemTr" style="display: 'table-row';">
  <td class="itemTableBody">[[${list.id}]]</td>
  <td class="itemTableBody"><a th:href="@{/show/{Id}(Id=${list.id})}">[[${list.title}]]</a></td>
  <td class="itemTableBody">[[${list.priority}]]</td>
  <td class="itemTableBody" th:with="index=${list.status}">[[${wordList.statusWord[index]}]]
                          <input type="text" th:value="${list.status}" style="display: none;"></td>
  <td class="itemTableBody" th:with="index=${list.category}">[[${wordList.categoryWord[index]}]]
                          <input type="text" th:value="${list.category}" style="display: none;"></td>
</tr>
```

### th:with
`index` に `${list.status}` の値が格納される。これを変数として使用可能。以下は添え字として使用。
```html
<td class="itemTableBody" th:with="index=${list.status}">[[${wordList.statusWord[index]}]]
```

### th:href
URLを生成する。以下の場合は `href="/show/{Id}"` が生成される。`{Id}` の部分が、`${list.id}` になる。
```html
<td class="itemTableBody"><a th:href="@{/show/{Id}(Id=${list.id})}">[[${list.title}]]</a></td>
```

### th:id
`id="pid{Id}"` に展開される。`{Id}` の部分は、`${list.progressId}` となる。
id を定義することで、JavaScript側から document.getElementById(id) で参照できる。

```html
<input type="checkbox" onclick="toggleProgressEditor(event)" th:id="'pid' + ${list.progressId}" class="customCheckBox1">
```

### th:for
`for="pid{Id}"` に展開される。`{Id}` の部分は、`list.progressId` となる。

```html
<label th:for="'pid' + ${list.progressId}" class="customCheckBoxL1"></label>
```

### th:utext
`list.contents` の内容をそのまま展開する(HTMLエスケープされない)。`th:text` の場合はエスケープされる。

```html
<p th:utext="${list.contents}" class="customContents1"></p>
```

```html
<p class="customContents1"> **ここに展開される** </p>
```


### csrfトークン
POSTを実施するときに必要なトークンを `hidden` でこっそりページに埋め込んでおく。
formをsubmitするときや、fetch() するときにこれらを取得して使用する。
`th:value="${_csrf.token}"` の部分が、`value="******"` となる。

```html
各ページの上部にあるlogout ボタン
<form method="post" action="/logout">
  <input name="_csrf" type="hidden" id="csrfToken" th:value="${_csrf.token}">
  <button type="submit" class="customButton2">logout</button>
</form>
```

上記のcsrfトークンをJSでも使用できる
```js
function postJson(url, data)
{
    return fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'X-CSRF-Token': document.getElementById('csrfToken').value // トークン取得
        },
        body: JSON.stringify(data)
    })
    .then((resp) => resp.json())
    .catch(error => {
        console.log(error);
    });
}
```


## Spring API
- https://spring.pleiades.io/spring-framework/docs/current/javadoc-api/

### 入力チェック

- @RequestBody
  - HTTPのBODYに入っているデータが `ConfigSystem` クラスとして引数となる
  - BODYではなくリクエストパラメータを受け取るには、`@RequestParam` を使用する

- @Validated
  - 入力チェックを自動でやってくれる
  - [BindingResult](https://spring.pleiades.io/spring-framework/docs/current/javadoc-api/org/springframework/validation/BindingResult.html) オブジェクトにエラー情報が入っている
  - @NotBlank, @NotNull, @Min, @Max, @Pattern などがある

```java
ハンドラ側
@PostMapping("/configSystem")          // POST http://hoge.com/configSystem
@ResponseBody                          // 戻り値はBODYとして送信される
public List<JsonResult> configSystem(@RequestBody @Validated ConfigSystem arg,
    BindingResult result, @AuthenticationPrincipal UserDetails detail)
{

    if(result.hasErrors()) // エラーチェック
        return bindingResultToJson(result, ret);
```

- @Data
  - privateメンバに対する setter/getter を自動生成する
  - setMember(), getMember(), toString() など (メンバ名の先頭が大文字になる)

- @AllArgsConstructor
  - 全てのフィールドを引数に持つコンストラクタを自動生成する

```java
@AllArgsConstructor // @Data とセットで使うことが多い
@Data
public class ConfigSystem {
    @NotBlank   // 空白を許可しない
    @Pattern(regexp = "^(jp|en)$", message = "Invalid format")
    private String lang;   // 正規表現パターンを指定

    @NotNull  // nullを許可しない(BODYの中にこのメンバが必須となる)
    @Min(Constants.ITEM_SORT_ID)       // 最小値を指定
    @Max(Constants.ITEM_SORT_DEADLINE) // 最大値を指定
    private Integer itemSortOrder;

    @NotNull
    @Min(Constants.PROG_SORT_NEW)
    @Max(Constants.PROG_SORT_OLD)
    private Integer progressSortOrder;
}
```

HTTP BODYの例<br>
ConfigSystem のメンバ名と、jsonのキーの名前を一致させる
```json
// RequestBody (JSON)
{"lang":"jp","itemSortOrder":"2","progressSortOrder":"0"}

// ResponseBody (List<>型なので、[] でくくられる)
[{"key":"itemSortOrder","value":"updated","attr":"INFO"}]
```

### URL + 数値

- [@PathVariable](https://spring.pleiades.io/spring-framework/docs/current/javadoc-api/org/springframework/web/bind/annotation/PathVariable.html)
  - URLに動的な数値が入る場合、その数値すべてに対しハンドラを定義することは事実上できない
  - `{ItemId}` という記述でまとめてハンドラを指定する
  - 数値は引数 `itemId` としてハンドラ内部で使用可能

```java
@GetMapping("/show/{itemId}")
public String show(@PathVariable Integer itemId, Model model,
    @AuthenticationPrincipal UserDetails detail)
{
```

### リダイレクト
- リダイレクトさせるときは、`"redirect:/path"` を返却する
- 下記の場合、ホスト名のみでアクセスしてきたら `/summary` に飛ばす

```java
@GetMapping("/")
public String index(@AuthenticationPrincipal UserDetails detail)
{
    loadConfig(detail.getUsername());
    System.out.println("[login] OK: " + detail.getUsername() + ", " + detail.getAuthorities());
    return "redirect:/summary";
    // addAttribute()せずにsummaryをそのまま返すことはできない
}
```

### ユーザー情報
- @AuthenticationPrincipal
  - [UserDetails](https://spring.pleiades.io/spring-security/site/docs/current/api/org/springframework/security/core/userdetails/UserDetails.html) オブジェクトからユーザー情報を取得可能

```java
ハンドラ側
@PostMapping("/configSystem")
@ResponseBody
public List<JsonResult> configSystem(@RequestBody @Validated ConfigSystem arg,
    BindingResult result, @AuthenticationPrincipal UserDetails detail)
{
    loadConfig(detail.getUsername()); // ユーザー名を取得できる
```


### 認証
- https://spring.pleiades.io/spring-security/site/docs/current/api/
- 認証部分の書き方はほぼ形が決まっている(が、ここにたどり着くまでが長い、、)
  - [EnableWebSecurity](https://spring.pleiades.io/spring-security/site/docs/current/api/org/springframework/security/config/annotation/web/configuration/EnableWebSecurity.html)
  - [HttpSecurity](https://spring.pleiades.io/spring-security/site/docs/current/api/org/springframework/security/config/annotation/web/builders/HttpSecurity.html)
- ログイン画面、認証ありのURL、認証なしのURL、admin限定URLなどを任意の値に変えられる

```java
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Autowired private AccountService accountServ;
    @Autowired private DataSource dataSource;
    @Autowired private ObjectPostProcessor<Object> objectPostProcessor;

    @Bean
    public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception
    {
        // pub配下は認証なしで許可
        // admin配下はADMINロールのユーザーのみ許可
        // 上記以外は認証が必要
        http.authorizeHttpRequests().requestMatchers("/pub/**").permitAll()
                                    .requestMatchers("/admin/**").hasRole("ADMIN")
                                    .anyRequest().authenticated();

        // カスタムURL=/loginとして全てのユーザーからのアクセスを認める
        // logout画面に全てのユーザーからのアクセスを認める
        http.formLogin().loginPage("/login").permitAll()
            .and()
            .logout().permitAll(); // これがないとlogout直後にまたリダイレクトされる
        return http.build();
    }
```


- ユーザー情報をDBから取得する場合。とりあえず動かしたいなら `UserDetailsService` でOK ([Sample](https://spring.pleiades.io/spring-security/site/docs/current/api/org/springframework/security/config/annotation/web/configuration/EnableWebSecurity.html))。
  - [AuthenticationManagerBuilder](https://spring.pleiades.io/spring-security/site/docs/current/api/org/springframework/security/config/annotation/authentication/builders/AuthenticationManagerBuilder.html)
  - [JdbcUserDetailsManagerConfigurer](https://spring.pleiades.io/spring-security/site/docs/current/api/org/springframework/security/config/annotation/authentication/configurers/provisioning/JdbcUserDetailsManagerConfigurer.html)
    - `usersByUsernameQuery` ユーザー名からユーザー情報を検索するためのクエリを設定
    - `authoritiesByUsernameQuery` ユーザー名からユーザー権限を検索するためのクエリを設定
    - `passwordEncoder` PasswordEncoder を指定
```java
    @Bean
    public AuthenticationManager authManager() throws Exception
    {
        AuthenticationManagerBuilder auth = new AuthenticationManagerBuilder(objectPostProcessor);
        // DBからユーザー情報を取得し認証を行うオブジェクトを生成
        auth.jdbcAuthentication()  // JdbcUserDetailsManagerConfigurer オブジェクトが得られる
            .dataSource(dataSource)
            .usersByUsernameQuery("SELECT name, pass, enabled FROM account_data WHERE name = ?")
            .authoritiesByUsernameQuery("SELECT name, role FROM account_data WHERE name = ?")
            .passwordEncoder(accountServ.getEncrypter());

        return auth.build();
    }
```

```java
@GetMapping("login") // 認証が必要な画面にアクセスするとここに飛ばされる(HTTP 302)
public String login()
{
    return "login";
}
```

login.html では user/passを入力し、"/login" に対してsubmit(POST)する。
```html
<form th:action="@{/login}" method="post">  ★↓csrfトークンが生成される
  <div>
  <input type="text" name="username" placeholder="Username"/>
  </div>
  <div>
  <input type="password" name="password" placeholder="Password"/>
  </div>
  <input type="submit" value="Log in" />
</form>
```

- 認証時のHTTP BODYは以下のような生データとなるので、本来は暗号化(SSL)が必要
```
_csrf=*****&username=***&password=****
```

- ログイン成立後はサーバーがCookieを渡し、以後、クライアントがサーバーに投げる(★)
```
   [Client]                       [Server]
       POST /login
       csrf&user&pass
       ------------------------------->

                               HTTP 302(http://192.168.0.7:8080/)
                               Set-Cookie: ★
       <-------------------------------

       GET /
       Cookie: ★
       ------------------------------->

                               HTTP 302(http://192.168.0.7:8080/summary)
       <-------------------------------

       GET /summary
       Cookie: ★
       ------------------------------->

                               HTTP 200
       <-------------------------------
```

### CrudRepository
- https://spring.pleiades.io/spring-data/commons/docs/current/api/
- DBアクセスするための基本的な処理は [CrudRepository](https://spring.pleiades.io/spring-data/commons/docs/current/api/org/springframework/data/repository/CrudRepository.html) にすでに実装されておりいつでも使える
- 追加で必要なIFのみを AccountRepository  に `@Query` として記述する
  - `@Param` でSELECT文に引数を設定できる
- リポジトリのインスタンスは `@Autowired` で宣言する

```java
public interface AccountRepository extends CrudRepository<AccountData, Integer>
{
    // :username の部分に、引数usernameの文字列が展開される
    @Query("SELECT * FROM account_data a WHERE a.name = :username") // ★1
    AccountData findByName(@Param("username") String username);

    // save, deleteById, existsById, findById などが含まれる
}

@Service
@Transactional
public class AccountServiceImpl implements AccountService { // ★2
    @Autowired AccountRepository repository; // new() しなくていい

    @Override
    public AccountData findByName(String name)
    {
        System.out.println("[Account] find id=" + name);

        AccountData ad = repository.findByName(name); // ★1 Queryが実行される
        return ad;
    }
```

```java
@SessionScope
@Controller
public class ActionItem {
    @Autowired private AccountService accountServ; // ★2

    private List<JsonResult> updateUser(ConfigAdmin arg, List<JsonResult> json)
    {
        AccountData ad = accountServ.findByName(arg.getUsername());

```

Service層のインスタンスも `@Autowired` にする。
これで、Controller、Service、Repositoryが繋がる。
```
[Controller]  ★2     [Service]          ★1        [Repository]
 ActionItem ------> AccountService(IF)
                    AccountServiceImpl ------> AccountRepository(IF) : CrudRepository
```

### DBのデータ構造
- `@Query` から `select *` したときに得られるデータ構造 `AccountData` は、DBのテーブルと合わせておく
  - メンバ名とテーブルのカラムを一致させる
  - DB側でアンダースコアの部分は大文字で連結される `item_sort_order` <--> `itemSortOrder`
- `key` となるメンバには `@Id` を付与する（手動でidを設定しないこと）
  - DB側は `serial primary key` とする
  - 手動で `psql` から追加するときは、このメンバだけを空にしておく

```java
@Query("SELECT * FROM account_data a WHERE a.name = :username")
AccountData findByName(@Param("username") String username);

public class AccountData {
    @Id
    private Integer id;

    private String name;
    private String pass;
    private String role;
    private Boolean enabled;

    private String lang;
    private Integer itemSortOrder;
    private Integer progressSortOrder;
}
```

```sql
create table account_data
(
  id serial primary key,
  name text NOT NULL,
  pass text NOT NULL,
  role character varying(12) NOT NULL,
  enabled BOOLEAN NOT NULL,

  lang character varying(4),
  item_sort_order integer,
  progress_sort_order integer,

  unique(name)
);
```

```java
// 新しいインスタンスを追加する場合は、`@Id` が付与されたメンバは `null` で追加する
// これで id が自動採番される
ad = new AccountData(null, arg.getUsername(), accountServ.encryptPassword(defaultPass),
        "ROLE_USER", true, "jp", 0, 0);
accountServ.save(ad);
```



