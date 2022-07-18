
## makefile いろいろ

`$@`
  ターゲットのファイル名

`$%`
  ライブラリの構成指定中の要素

`$<`
  最初の必須項目のファイル名

`$^`
  すべての必須項目のファイル名(重複は取り除かれる)

`$+`
  すべての必須項目のファイル名(重複を含む)

`$?`
  ターゲットよりも後で更新された必須項目のすべて

`$*`
  ターゲットファイル名の一部(サフィックスを除いたファイル名)

`=`
  再帰変数を作成(代入後、右辺が更新されると左辺も更新される)

`:=`
  単純変数を作成(左辺はその後、更新されない)

`?=`
  左辺が値を持っていないときに代入される(左辺は再帰変数扱い, :=ではなく=と同じ)

`+=`
  左辺の値を保持して末尾に追加する(左辺は再帰変数扱い, :=ではなく=と同じ)

<br>

## ルール/関数
makeコマンドを実施したときに、$(TARGETS)が生成される<br>
$(TARGETS)に必要なもの(依存関係)を順に遡って、最初から処理が始まる<br>
別に'all'じゃなくてもいいが、allがよく使われる(allを最初に書くこと)<br>
ただし、all というファイルがすである場合、makeが走らない(.PHONY: all を事前に書けば回避できる)
  ```makefile
  .PHONY: all
  all: $(TARGETS)
  ```

aaa.o を生成するために、aaa.cppが必須とされる(依存している)<br>
aaa.cpp が更新されると、aaa.o が再生成される<br>
フォルダにあるすべての\*.cppが対象となる
  ```
  %.o: %.cpp
  ```

コロン(:)を2回区切ると、ターゲットを絞れる<br>
XXXに含まれるファイルのみがこのルール(\*.cpp --> \*.o)の対象となる
  ```makefile
  XXX: %.o: %.cpp
  ```

<br>
サフィックスなしの特別ルール<br>
hoge.cpp をビルドしたら、hoge を生成することになる

  ```makefile
  %: %.cpp
  ```

$(FILES2)に含まれるすべての \*.cpp が、\*.exe に変換される
  ```makefile
  TARGET2 = $(FILES2:.cpp=.exe)
  ```

FILES_ALL に含まれるすべての \*.cpp が、\*.d に変換され、さらに先頭に TMPDIR が追加される
  ```makefile
  DEPENDS1 = $(addprefix $(TMPDIR)/, $(FILES_ALL:.cpp=.d))
  ```

<br>

findコマンドを使用して、SYM_DIRの配下にある \*.o を OBJS1 に格納する
  ```makefile
  OBJS1 := $(shell find $(SYM_DIR) -name "*.o")
  ```

環境変数HOGE_ENVの値によって処理を切り替える
  ```makefile
  ifeq ($(HOGE_ENV), hoge)  # 事前に export HOGE_ENV=xyz を行う
   HOGE = "abc"
  else
   HOGE = "def"
  endif
  ```

<br>

## Sample1:
カレントディレクトリの *.cpp をすべてビルドして実行ファイルにする
```makefile
CXXFLAGS  = -O3 -std=c++11 -Wall -Wno-unused-value -Wno-unused
#FILES1    = $(shell find *.cpp) #どちらもOK
FILES1    = $(wildcard *.cpp)
TARGETS   = $(FILES1:.cpp=)
LIB1      = -lpthread

all: $(TARGETS)

# @をつけるとコマンドの出力結果が抑制される(echoの場合は出力が被るのを防ぐ)
%: %.cpp
	@echo "  [CXX] $^"
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LIB1)

clean:
	rm -f $(TARGETS)
```

## Sample2:
再帰的なmakeを行う場合のトップのMakefile<br>
```makefile
#環境変数でクロス環境を指定する(必要に応じて)
CXX        = $(CPU)g++
CC         = $(CPU)gcc
AR         = $(CPU)ar
LD         = $(CPU)g++

#配下のMakefileに値を継承する
export CC AR LD CXX

SUBDIR_ALL =  test plugin

#このMakefileでは、test, plugin フォルダに潜るだけ(同じ階層には、ソースファイルを配置しない)
#階層が2段で終わる場合は、test, plugin の中に、普通のMakefileを書く
all:
	for n in $(SUBDIR_ALL); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIR_ALL); do $(MAKE) clean -C $$n || exit 1; done
```

<br>

## オブジェクトを別フォルダに生成

(*.cpp が配置されているフォルダとは別の場所に *.o, *.d などを生成する場合)

  生成する*.dファイルにTMPDIRをプレフィクスとして追加<br>
  gcc/g++ -MM で生成された *.d にはフォルダ名がない(hoge.o: ~~ を $(TMPDIR)/hoge.o: ~~ にする ★)<br>
  ```makefile
  $(TMPDIR)/%.d: %.cpp
	@mkdir -p $(TMPDIR)    # すでに作成済みの場合は無視(-p)
	@echo "  [MM] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -MM $< > $@
	@sed -i -e "s/\(.*:\)/$(TMPDIR)\/\1/" $@ ★

  変換前
    Timer.o: Timer.cpp Lock.h Timer.h Log.h Factory.h
  変換後
    .objs/Timer.o: Timer.cpp Lock.h Timer.h Log.h Factory.h
  ```

  サフィックスルールの左にTMPDIRを使う( *.cpp をビルドして、TMPDIRの下に *.o を生成 )
  ```makefile
  $(TMPDIR)/%.o: %.cpp
	@echo "  [CXX] $<"
	@$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
  ```

  生成対象となるオブジェクトにTMPDIRをプレフィクスとして追加
  ```makefile
  OBJS2 = $(addprefix $(TMPDIR)/, $(FILES2:.cpp=.o))
  ```

  ターゲットの依存部分に OBJS2を使う（プレフィクスがついている *.o )
  ```makefile
  $(TARGET1): $(OBJS1) $(OBJS2)
	@echo "  [LIB] $@"
	@$(LD) $(LDFLAGS) $^ -lgcov -o $@
  ```


