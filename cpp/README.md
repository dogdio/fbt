
## C++ サンプル集

ビルド方法
```
$ cd cpp
$ export WITH_GCOV=1 or 0
$ make
```

---

### instance-manager
複数の共有ライブラリを読み込み、一斉に初期化/終了するサンプル

### job
コマンドパターンのサンプル

### misc
言語仕様の細かい部分の確認(++11など)

### sample-gcov
test-toolとgcovを動かすためだけの最小構成サンプル

```
$ cd sample-gcov
$ ../test-tool/ExecTest -d ./
===== SampleTest =====
[1.1.1] OK
[1.2.1] OK
[1.2.2] OK
[1.3.1] OK
[1.3.2] OK
[1.3.3] OK

result: OK

$ ./gcov.sh
./aaa に出力される
```

### stubgen
C/C++のスタブを生成するツール(完壁なものではない...)<br>
下記をインストールしてからビルドする

```
$ sudo apt-get install libssl-dev
$ sudo apt-get install libpcap-dev
```

---

### test-tool
単体テストのフレームワーク<br>
テスト対象は共有ライブラリとしておく
```
$ cd test-tool
$ ls plugin
Makefile  Sample2.so  Sample.so  TestSample2.cpp  TestSample.cpp

Sample.so, Sample2.so のテストを実施
$ ./ExecTest -d ./plugin
===== HOGE2 =====
[2.1.1] OK
[2.1.2] OK
[2.1.3] OK
===== HOGE =====
[1.1.1] OK
[1.1.2] OK
[1.1.3] OK

result: OK
```

doxygenでドキュメントを生成できる<br>
```
$ sudo apt-get install doxygen graphviz mscgen

$ make doxpub  # testフォルダが除外される
$ make doxpriv # 全モジュールが出力される
./html に出力される

$ vimdiff doxygen-pub.conf doxygen-priv.conf # 詳細な差分はこれで確認
```

### utils
重要なパターン、パッケージングなどをまとめたサンプルライブラリ<br>
utilsのテストはtest-toolにて実施可能(テスト結果はgcovで計測できる)

```
$ cd utils
$ ./test.sh
$ ./gcov.sh # WITH_GCOV=1 の場合のみ

./aaa に出力される
$ ls aaa/coverage.html
aaa/coverage.html

```
