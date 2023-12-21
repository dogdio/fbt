
# doxygen/graphviz/mscgen

https://www.doxygen.nl/manual/index.html<br>
https://www.doxygen.nl/manual/commands.html<br>

- 関数のヘッダだけでなく、ファイルの先頭に説明や図を入れて詳細なドキュメントが作れる
- ソースやヘッダとは独立したページ `@page` を定義し、いろいろ記述したりジャンプできる
- ソースコードの一部を抜粋して、図と合わせて説明可能
- 生成した図から別の図、定義位置などにジャンプできる

---

### 箇条書き

`@ref ***` の部分がハイパーリンクになってファイルやクラスに飛べる<br>
`@note` 以外にも、`@pre`, `@post` などがある
```
    ### Operation Overview
    -# Test framework initialization
    -# Parse command line parameters
       - Set configration
    -# Load shared library
       - Install User Instance (see @ref TestSample)
       - Also load any other required libraries here (see @ref TestSample4.cpp)
    -# Traverse Instance queue
       - Registration Tests
       - Execution Tests
    -# UnLoad shared library
    -# exit

    @note
      List of illustrations following (described manually). \n
      Click on the sequence diagram to jump to the next diagram(functions) \n
```
![bullet-points](bullet-points.png "bullet-points")

---

### @attention の中に箇条書き等を入れ込む

```
/** @file TestSample4.cpp
    @brief Sample Test class 4
    @attention
    If libSample4.so depend on libSample4Ex.so: \n
    -# When build libSample4.so, you must specify the external depends library (use -l option: -lSample4Ex) \n
    -# you must set Environment variable (please do the following) \n
    ```
    $ export LD_LIBRARY_PATH=./plugin2
    $ ./ExecTest -d ./plugin2
    ```
    @see TestSample4Ex.cpp */
```

![attention](attention.png "attention")

---

### 関数コメント
`@mscfile` で記述したシーケンスが画像として表示される<br>
画像に表示された関数名などはクリック可能でジャンプできる

```
ヘッダファイル
/** Called once for registration your test code \n
    please call Register() many times \n
    @retval true test will be processed
    @retval false test is not processed

    @mscfile Regist.msc "regist sequence" */
virtual bool RegisterTests(void) = 0;

実装側(コメントなし)
bool TestSample::RegisterTests(void)
{
	Register("1.1.1", test_1_1_1);
	Register("1.1.2", test_1_1_2);
	Register("1.1.3", test_1_1_3);
	return true;
}
```

![function](function.png "function")

◆mscgen(*.msc)の書き方<br>
`|||` 1行開ける(実線)　
`...` 1行開ける(点線)　
`=>` 矢印(実線)　
`>>` 矢印(点線)　
`url` ジャンプ先(@ref)を定義　

```dot
msc {
  A[label="TestFramework"], B[label="YourCode"];

  |||;
  A box A [label="Test Registration", textbgcolor="#ccffcc"];
  A=>B [label="RegisterTests()", url="@ref test::TestBase::RegisterTests"];

  |||;
  A<=B [label="Register()", url="@ref test::TestBase::Register"];
  A>>B [label="return"];
  A<=B [label="Register()", url="@ref test::TestBase::Register"];
  A>>B [label="return"];

  ...;

  |||;
  A<<B [label="return"];
}
```

---

### ページを定義して、そのページにジャンプ
`@page` でページを定義できる。ページ内に手書きの図、説明などを入れることが可能。

```
    ### Sequence diagram
    - @ref page10      ★
      -# @ref page11
      -# @ref page12
      -# @ref page13

    @cond SECTION_PRIVATE
      ### Block diagram
        -# @ref page01    ☆

      ### Data structure
        -# @ref page02
    @endcond

/** @page page10 All Sequence  ★
    @mscfile All.msc "All sequence"
    @see test::TestBase */

/** @cond SECTION_PRIVATE
      @page page01 Modules Overview    ☆
      @dotfile TestMain.dot
      @see test::TestMain
      @see test::TestBase
    @endcond */
```

◆以下の設定が有効な場合のみ、上記の `@cond` ～ `@endcond` の領域が展開される
```
ENABLED_SECTIONS       = SECTION_PRIVATE
```

![link](link.png "link")

---

### ソースコードの内容を一部抜粋する
`@dontinclude` で指定したファイルの一部を取り込める<br>
`@skip` で指定した文字列から、`@until` で指定した文字列が見つかるまでが対象範囲となる<br>
変数などはジャンプ可能。

```
  - @ref TestBase.cpp
    @dontinclude TestBase.cpp
      @skip namespace {
      @until };
```

![include](include.png "include")

◆ `@snippet` を使う場合<br>
下記を読み込む側のコメントに記載する（同一ファイル内で読み込みは可能）。<br>
```
@snippet TestConfig.h Snippet1                            
```

読み込まれる側(TestConfig.h)に目印をつけておく
```
//[Snippet1]
     :
     :
//[Snippet1]
```



---

### dotfileサンプル(top to bottom)
`rankdir`が`TB(top to bottom)`だと下に向かって進む<br>
`URL=***` の部分に、`@ref` を使えばリンク可能な図となる
（anonymousな空間の場合はちょっと長くなる）<br>
```dot
digraph g {
  rankdir=TB;
  graph [dpi="64"];
  node [shape=box ];
  Top     [label="Caller", color=black];

  GetInst [label="config::GetInstance()", color=red URL="@ref test::config::GetInstance"];
  AbsSet  [label="ConfigIF::Set()", color=black URL="@ref test::config::ConfigIF::Set"];
  AbsGetS [label="ConfigIF::GetString()", color=black URL="@ref test::config::ConfigIF::GetString"];
  AbsGetI [label="ConfigIF::GetInteger()", color=black URL="@ref test::config::ConfigIF::GetInteger"];

  Inst [label="Instance1", color=red ];
  Set  [label="TestConfig::Set()", color=red URL="@ref anonymous_namespace{TestConfig.cpp}::TestConfig::Set"];
  GetS [label="TestConfig::GetString()", color=red URL="@ref anonymous_namespace{TestConfig.cpp}::TestConfig::GetString"];
  GetI [label="TestConfig::GetInteger()", color=red URL="@ref anonymous_namespace{TestConfig.cpp}::TestConfig::GetInteger"];

  Top -> GetInst [label=" 1.Get instance", dir="both", fontcolor=orange, color=orange];
  Top -> AbsSet;
  Top -> AbsGetS [label=" 2.Abstract IF"];
  Top -> AbsGetI;

  GetInst -> Inst [style="dotted", dir="back"];
  AbsSet  -> Set [penwidth="2", color=blue];
  AbsGetS -> GetS [penwidth="2", color=blue, label=" Inheritance", fontcolor=blue];
  AbsGetI -> GetI [penwidth="2", color=blue];

  memo1 [label="TestConfig.cpp", fontcolor=red, shape="plaintext", style="" ];
  AbsGetI -> memo1 [style="invis"];
}
```

![config](config.png "config")



