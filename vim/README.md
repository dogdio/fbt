
# vim

[vim日本語ドキュメント](https://vim-jp.org/vimdoc-ja/)

★ Help参照
```
:help
:help usr_xx
```

<br>

## Concept
**ヤンクしたデータとexコマンドを組み合わせて、入力を自動化したものをmapで割り当てる**<br>
**使用頻度が高く、利用価値の高いコマンドを .vimrc に map しておく**

★カーソル位置の文字列を取得<br>
現在のカーソル位置の文字列（単語）をヤンク<br>
```
yiw
```

exコマンドで、`Ctrl+r`の後に、`”`を入力
（現在のカーソル位置の文字列をコピーして、、、続けてexコマンドで何かを行う）
```
:<C-r>”
```

検索した文字列の取得
```
:<C-r>/
```

上記を利用して、新規コマンドを作成（ヤンクしたデータの取得 + exコマンド）

<br>

★ 大量のコードをどうやって読み書きするか？（all.txt ）<br>
あらかじめ自分の見るべきファイルをリストアップしておく<br>
このリストされたファイル(all.txt)経由で、見たいファイル(*.c, *.h)を開く<br>
ターミナルから `vim xx/yy/zz.c` などとはいちいちやらない！<br>
**このリスト化されたファイルの一覧をtag-jump, grep の対象とする（IDEのワークスペース）**<br>
膨大なディレクトリ群から見るべき対象を絞り込み、一覧を生成する。[生成方法](https://github.com/dogdio/fbt/blob/master/vim/bin/All.sh)は何でもOK。<br>
ディレクトリ名が長い場合は、フォルダを掘って、その中からシンボリックリンクを使う<br>
自分の担当範囲と共通ライブラリ、下位層、上位層当たりを一覧にしておく<br>
```
$ find dir1/ -name “*.[ch]”  > all.txt
$ find dir2/ -name “*.cpp”  >> all.txt
```

```
関係するコードは全てall.txtに記載し、ボタン一発でgrep, tag-jumpできるようにする
  [UpperLayer]
       ↓
  [  My Code ] <--> [Common Library]
       ↓
  [UnderLayer]
```

all.txtを開いて、縦分割 `vs` する<br>
自分が開きたいファイルをvim上で検索する。`/file<CR>` とすることで、目標物に素早くたどり着く。<br>
ファイルブラウザ、コンソール画面からファイルを探す必要は無い。<br>
カーソルを左において、`Ctrl+f`でファイルを右側に開く（左側には一覧がキープされる）<br>
左側に開く場合は、`Shift+f` 、hoge.c を見ているときに、all.txtに戻るときは、`Shift-A`
```
$ vim all.txt
:vs
/hoge.c
```

```
" .vimrc
map <C-f> 0y$<C-w>l:open <C-r>"<CR>
map F 0y$:open <C-r>"<CR>
map A :open all.txt<CR>
```

![vim1](https://github.com/dogdio/fbt/blob/for_img/img/vim1.png "all.txt")

「左側に一覧`all.txt`、右側にコード`Lock.h`」、のように並べて表示すると見やすくなる<br>
grep結果は右側の下半分に表示される（詳細は後述）<br>
左側でメモをとりつつ、右側にコードを表示するということもできる<br>
all.txt に新しいファイル名を追記して、`Ctrl+f`で新規ファイルとしてオープンできる<br>

★ カーソル位置のファイルを開く<br>
`gf`でカーソル位置のファイルを開くことができる。上記の`F`と同じ。<br>
systemのインクルードファイル(stdio.h, unistd.hなど)を`gf`で開くことができる<br>
（後述の、`Ctrl+b`で戻れる）<br>
`Ctrl+w gf` で新しいタブでファイルを開く。`Ctrl+w f` だと水平分割して開く<br>

★ all.txt を新しいタブで再度開く<br>
**新しいタブを開いて、all.txt を開いて、新しいファイルを開く、、、これは何度も繰り返される作業なので、ボタン一発で出来た方がよい**<br>
`Ctrl+a`により、`新しいタブの左側にall.txt, 右側に現在のファイルが再表示` される

```
map <C-a> mA:tabnew all.txt<CR>:vs<CR><C-W>25<<C-W>l'Azz
```
  
このコマンドは、`あるファイルを参照しているときに、現在のポジションをキープしたまま、同ファイル内の別の箇所を参照する` という目的にも使える。<br>
上記の`Ctrl+a`で新しいタブを開いた場合、`:lopen`すると、location-listは継承されるが、以前のタブとは別物として扱うことが出来る

<br>

## Move, Search
★ カーソルの移動、タブの移動、マウス<br>
上`k`, 下`j`, 左`h`, 右`l` のキーバインディングを、タブ、ウィンドウの移動にも適用する<br>
`Ctrl+l` で左のタブ、`Ctrl+h` で右のタブに移動する<br>
新しいタブでファイルを開くときは、`:tabnew hoge.c` 
```
map <C-l> :tabnext<CR>
map <C-h> :tabprev<CR>
```

縦に分割する場合は、`:vs`, 水平に分割する場合は`:sp`<br>
`Ctrl+j`で分割したウィンドウの下、`Ctrl+k`で上に移動する<br>
（Ctrl+wを省くだけだが楽になる）
`Ctrl+w h`, `Ctrl+w l` で縦分割したウィンドウを左右に移動する
```
map <C-j> <C-w>j
map <C-k> <C-w>k
```

マウスでクリックした位置にカーソル移動する（タブも選択できる）<br>
```
set mouse=a
```

TeraTermで文字列を選択時にコピペをしたい場合は、`:set mouse= `にすることでコピーできるようになる<br>
(この状態だとクリック位置にカーソルは合わない）

分割したウィンドウの幅調整<br>
`F9`, `F10`で縦幅、`F11`, `F12`で横幅を調整する
（縦幅調整後はカーソルは下ウィンドウ、横幅調整後は右ウィンドウになる）
```
map <F9> <C-j><C-W>3+
map <F10> <C-j><C-W>3-
map <F11> <C-w>l<C-W>3>
map <F12> <C-w>l<C-W>3<
```

ファイルの先頭 `gg`、ファイルの末尾`G`、行の先頭`0`、行の末尾`$`に移動<br>
関数の先頭に後方移動 `{{`、前方移動 `}}`<br>
対応する括弧、#if ~ #endif に移動 `%`<br>


★ 以前のカーソル位置に戻る、進む<br>
デフォルトは `o, i` だがやや覚えにくいので、割り当てを変えている。<br>
既存コマンドを再割り当てするときは、noremapを使う<br>
戻るとき(back)は`Ctrl+b`、進む(next)ときは、`Ctrl+n`
```
noremap <C-b> <C-o>
noremap <C-n> <C-i>
```

ファイルを開いたときに以前のカーソル位置を復旧させる
```
:au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
```

***

★ 検索<br>
カーソル位置で `*`で、単語の境界有りで前方検索。`n` で続けて前方検索、`N`だと後方検索<br>
単語境界有りの場合は、`/\<hoge\>` のように、括る

<br>

検索した文字の色づけのON/OFF
```
set hlsearch
set nohlsearch
```

インクリメンタル検索しない
```
set noincsearch  
```

大文字小文字を区別して検索する
```
set noignorecase
```

現在のカーソル位置から、単語の末尾までを検索対象とする、`Shift+y`
```
map Y ye/<C-R>"<CR>
```

現在のハイライトを非表示にする、`q`
```
map q :noh<CR>
```

前回検索した文字に加えて新たな検索文字として、カーソル位置のワードをORで追加する、`F3`<br>
複数のキーワードをハイライト表示したいときに使用する<br>
ハイライトされるキーワードは全てのタブ、ウィンドウで同じである<br>
```
map <F3> yiw/<Up>\\|\<<C-R>"\><CR>
```

左側のカーソル位置にある単語を、右側で検索する、`F7`<br>
`F8` と使用方法はほぼ同じ。`F7`の後に、`n`を連打で、検索を継続できる<br>
関数定義ではなく、関数の参照箇所を検索する場合はこちらのほうがよい<br>
```
map <F7> yiw<C-w>l/\<<C-R>"\><CR>
```

***

★ tag-jump<br>
all.txt が存在するディレクトリに、tagsファイルを生成しておく。<br>
（all.txtに含まれるファイル全てをtagsの対象とする）
```
$ ctags `cat all.txt`
$ vim all.txt
```

tag-jumpする場合は、`Ctrl+]`、戻る場合は`Ctrl+t`<br>
画面を水平に分割しつつtag-jumpをするときは、`Ctrl+w Ctrl+]`<br>
`zz`で中央に表示する
```
map <C-t> <C-t>zz
map <C-]> <C-]>zz
```

指定したキーワードに対して、tag-jumpをする<br>
名前はTABで補完可能。stagだと画面分割有り（vtagはない）<br>
```
:tag hoge_func
:tag hoge_ --> このあとTABで補完できる
:stag hoge_func
```

tag-jump先が複数ある場合（同じ名前の定義が複数存在する）
```
Ctrl+]
:ts
番号を選択して、<Enter>
```

タグファイルを更新する、`Shift+T`
```
map T :!ctags `cat all.txt`<CR>
```

新しいタブを開いてtag-jumpする、`Ctrl+o`
```
map <C-o> yiw:tabnew <CR>:tag <C-R>"<CR>zz
```

現在のカーソル位置から単語末尾までに対して、tag-jumpする、`Ctrl+y`
（関数の前のアンダースコアなどのプレフィックスが邪魔な時）
```
map <C-y> ye:tag <C-R>"<CR>zz
```

★ **縦分割状態で、左側のキーワードを右側でtag-jumpして開く**、`F8`<br>
左側でメモファイル、ヘッダファイルを開きつつ、右側でソースを開く、のように使える。<br>
または、`左側が関数の呼び出し側、右側が呼び出される側` のようにすると非常に見やすい。<br>
（名前が長すぎてコードが横長になってしまうと縦分割がやりにくいのでほどほどの長さにすること）
```
map <F8> yiw<C-W>l:tag <C-R>"<CR>zz
```

![warn1](https://github.com/dogdio/fbt/blob/for_img/img/warning-min.jpg "warn1") **注意！！**<br>
tag-jump出来ない場合は、all.txt とは別ファイルで定義されている、もしくは糞マクロにより定義部がパースできていない、
外部のライブラリに定義がある、、等が考えられる。
マクロが原因の場合は、そのようなマクロは避けた方がいい（関数の定義部分をマクロにしちゃう、、）。
基本的に、ctagsでパースできないようなコードは書くべきでは無い（メリットよりもデメリットの方が多い）

<br>

***

★ grep(vim 内部grep)<br>
grepの対象をall.txtに記載されているファイル全てとする（検索対象が見つからない場合は、all.txtのリストを見直す）<br>
vimはデフォルトで内部grepを起動するので、grepprgであらかじめ外部コマンドを割り当てておく<br>
grepはcgrep, lgrepの2種類があるが、基本はlgrep だけでおｋ
```
set grepprg=Grep2
```

[Grep2](https://github.com/dogdio/fbt/blob/master/vim/bin/Grep2) コマンドはall.txtの全ての要素に対して、grepを仕掛けるだけでOK
```shell
#!/bin/sh
cat all.txt |xargs grep -nE --color $@
```

**カーソル位置にある単語を境界ありで、lgrepする** `F6`<br>
grep結果を参照するときは、`:lopen` （省略すると `:lop`）
```
map <F6> yiw:lgrep -w <C-R>"<CR>:lop<CR>
```

**選択した範囲をlgrepする** `F5`
`Ctrl+v` などで選択した状態から `F5` を押す
```
map <F5>   y:lgrep    <C-R>"<CR>:lop<CR>
```

★ lfile, cfile(外部grep結果の取得)<br>
`lfile` `cfile` は外部コマンドで行った、grep出力結果をvimに取り込む（ファイル名、行番号が含まれていること）<br>
取り込んだ結果に対して `Enter` もしくは、ダブルクリックでジャンプできる (`:set mouse=a` しておくこと)<br>
**`l` は各タブごとに確保されるが、`c` は全体で共有される。基本的には `l` を使う**<br>
`lfile` で先頭の要素にジャンプし、`lopen` で一覧(location-list)を画面下に表示する
```
$ Grep2 aaa | grep -v bbb > hoge.txt
:lfile hoge.txt
:lopen
```  

★ 開いているファイル内のgrep<br>
grepprgはGrep2が既に設定されているので、外部grepを使用するMygrepを作る（毎回、grepprgを再設定するのもあり）<br>
カーソル位置の単語を、現在開いているファイルのみでlgrepする `Ctrl+g`
```
map <C-g> yiw:Mygrep <C-R>"<CR>
```

```vim
if !&cp && has("user_commands")
command! -nargs=* Mygrep call MYGREP(<f-args>)
function! MYGREP (pattern)
	let tmp = ":!grep -EHn " . a:pattern . " " . expand("%p") .  " > ~/.vim/ttt"
	execute tmp 
	unlet tmp
	execute ":lfile ~/.vim/ttt"
	execute ":lopen"
endfunction
endif
```

★ 指定したファイルをall.txtから探す(include fileなどを探す)<br>
カーソル位置にあるファイルをall.txt から探して、左側に表示する `F2`<br>
拡張子までを含めてヤンクするために `b` で単語の先頭に戻っている（単語の先頭文字で `F2` を入力できないので注意）
```
map <F2> b<C-v>eeey<C-w>l:open all.txt<CR>/<C-r>"<CR><C-f>
```

## Edit

◆ 入力補完<br>
インサートモードでキーワードを途中まで入力してから `Ctrl+n`, `Ctrl+p` <br>
次の候補に移動するときは、`Ctrl+n` 戻るときは `Ctrl+p` <br>
補完を完了するときは `ESC` で抜ける<br>

◆ Undo/Redo<br>
`u` でUndo, `Ctrl-r` でRedo<br>
Undoすると、1つ前の編集位置に戻る（カーソル位置を元の位置に戻す目的でも使える）<br>

◆ 以前の編集操作を繰り返す<br>
何か編集してから、`.` （ピリオド）<br>
移動コマンドの後に `.` を連続して交互に押して高速に処理できる<br>
（`j` で下に移動して、`.`で貼り付け、、を繰り返す、等）<br>

◆ 範囲選択<br>
`Shift+v` のあと上下移動で、1行ずつ選択範囲を増やせる<br>
`Ctrl+v` だと、矩形選択が可能。1行の途中まで選択も可能<br>
`v` だと矩形選択はできないが、1文字ずつ選択範囲を広げられる（行の途中から次の行の途中まで、が可能）<br>
<br>
選択範囲が決まったら、`y`でヤンクして、`p`で貼り付ける。（`d`で切り取りも可能）<br>
矩形選択された文字列は矩形としてペーストされる<br>
`p`を押した位置を起点として、X,Y方向に展開される（足りない部分は空白が付与される）<br>

現在の括弧の中全部（スコープ）を選択するコマンド
```
vi{
```
  
★ 文字列置換（現在編集中のファイル）<br>
現在開いているファイルで、カーソル位置の単語に、単語境界ありで置換処理を行う `S`、境界無しの場合は `s`<br>
最後にエンターを押すまで置換処理は走らない。置換後の文字列を手動で設定してから `Enter` を押す（置換前の文字列をベースにして設定する）。<br>
1行に処理を繰り返して行う場合は、`/g` を付与する
```
map s yiw:1,$s/<C-R>"/<C-R>"
map S yiw:1,$s/\<<C-R>"\>/<C-R>"
```

```
:1,$s/\<before\>/after/g
:1,$s/\<before\>/after
:1,$s/before/after/g
:1,$s/before/after
```

行数を指定する場合(100行目から200行目、現在の行から200行目）
```
:100,200s/before/after
:.,200s/before/after
```
  
★ 文字列置換（複数ファイル）<br>
置換したいキーワードをあらかじめgrepしておく（location-listにあれば何でもOK） <br>
**location-listに表示されている項目に対して、置換処理を行う** `c`<br>
変換したいキーワードにカーソルを合わせて `c` を入力し、置換後の文字を決めて、`Enter` する
```
:MyConvert before after
map c yiw:MyConvert  \<<C-R>"\>  <C-R>"
```

```vim
if !&cp && has("user_commands")
command! -nargs=* MyConvert call MYCONVERT(<f-args>)
function! MYCONVERT (v1, v2)
	let loclist = getloclist(0)
	let target = "!@#$"
	let filenum = 0
	for list in loclist
		"echo bufname(list.bufnr) ':' list.lnum '=' list.text
		if bufname(list.bufnr) !=# target
			if filenum !=# 0
				execute ":w"
			endif
			execute ":open " bufname(list.bufnr)
			let filenum = filenum + 1
		endif

		let tmp = ':' . list.lnum . ',' . list.lnum . 's/' . a:v1 . '/' . a:v2 . '/'
		execute tmp
		unlet tmp
		let target = bufname(list.bufnr)
	endfor
	
	execute ":w"
	unlet loclist
	unlet target
	unlet filenum
endfunction
endif
```

**lgrepの結果(location-list) の編集**<br>
置換前などに不要なものを削除できる<br>
`modifiable` でgrep結果が編集可能となり、`lbuffer` で編集結果が反映される(`dd` で不要な行を消す）
```
:set modifiable
dd
:lbuffer
```

★ Copy/Paste（プロセス跨いだコピペ）<br>
タブ、ウィンドウを跨いでコピペする場合は `yy` `p` でOK<br>
ターミナル1のvimからターミナル2のvimへコピペを行う場合は `yy` `Ctrl+c` `Ctrl+p` とする<br>
<br>
手順は以下の3つ（`yy` `Ctrl+c` までがターミナル1、`Ctrl+p` を別のターミナル2で実行）
```
ターミナル1
何かしら文字列をヤンクする(Shift+v から yy など)
<C-c> :!rm -f ~/.vim/ttt<CR>:new ~/.vim/ttt<CR>p:wq<CR>
```
```
ターミナル2
<C-p> :r ~/.vim/ttt<CR>
```
当然ながら、上記のやり方では、ホストを跨いだコピペは出来ない<br>
TeraTermで、コピペを行う場合は、インサートモードにして `set mouse=` の状態（aをはずす）で、貼り付けを行うこと。<br>
TMPファイルを `~/tmp/hoge.txt` 配下にコピーして、それを `:r ~/tmp/hoge.txt` で読み取れば良いだけ。<br>

★ Diff<br>
`diff -rq` で差分の一覧を表示させ、そのファイルをvimで開く。<br>
差分を見たいファイルにカーソルを合わせて `:Diff`
```
$ diff -rq before/ after/ > hoge.diff
$ vim hoge.diff
カーソルを上下に移動(Files aaa.c and bbb.c differ のリストから選ぶ)
:Diff
```

差分が記述されている箇所はカーソルを合わせることが出来る（削除された領域はカーソルを合わせることは出来ない）<br>
差分を適用する場合は `dp`、差分を受け取る（削除する）場合は `do`<br>
下の差分に移動する場合は `]c`、上の差分に移動する場合は`[c`<br>
編集中に差分を更新して再描画する場合は、`:diffu`<br>
Undo/Redoは普段通り使える。all.txt と同じディレクトリで、差分ファイルを開けば、普段と同じ作業がそのまま可能となる<br>
```vim
if !&cp && has("user_commands")
  command -range=% Diff :call DIFF()
  func DIFF()
    let line = getline(".")
	let mx='Files\s\(\f\+\)\sand\s*\(\f\+\)'
	let l = matchstr(line, mx)
	let file1 = substitute(l, mx, '\1', '')
	let file2 = substitute(l, mx, '\2', '')
	let exe1 = "tabnew " . file2
	let exe2 = ":vert diffsplit " . file1

	execute ":set scrolloff=3"
	execute exe1
	execute ":set syntax=off"
	execute exe2
	execute ":set syntax=off"

	unlet line
	unlet mx
	unlet l
	unlet file1
	unlet file2
	unlet exe1
	unlet exe2
  endfunc
endif
```

set diffopt によって、Diffの設定を変えられる<br>
削除領域を黒で埋める(filler)、空白の差分を無視する(iwhite)。

## etc

★ カーソル位置にラインを引く <br>
set cursorline しておかないと、hi CursorLine は有効にならない<br>
```
set cursorline
```

色設定
```
hi CursorLine      cterm=none ctermbg=246
```

★ 外部コマンド(git/svn/make) 連携<br>
現在作業中のリポジトリに対して、Diffをとる `Q`<br>
all.txt を開いているディレクトリがリポジトリのトップである場合は単純だが、リポジトリが複数ある場合は細工が必要<br>
svnのところをgit に変えてもOK。TMPファイルに吐き出してそれを新しいタブで読み取るだけ
```
Q :!svn diff > hoge.diff<CR>:tabnew<CR>:r hoge.diff<CR>:set syntax=diff<CR>
```

Make環境のトップディレクトリにall.txt を配置しておいて、makeもvimから行う、`M`
```
M :lmake<CR>:lopen<CR>
```

★ man結果の取り込み<br>
カーソル位置のキーワードに対して、man を行うのが `Shift+k` である<br>
この結果はデフォルトではlessに渡されており、vimの操作ができないため実は不便<br>
**以下の設定で、新規タブに結果を取り込める（新規タブから、引数の型、必要な includeファイル名等をコピペできる）**
```
map K yiw:!man.pl <C-R>" <CR>:tabnew<CR>:r ~/.vim/mmm<CR>:set syntax=man<CR>/<C-R>"<CR>:set nonu<CR>
```
manの出力結果は行数を含んでいないので、`:set nonu` しないと行末が少し欠けてしまう

```perl
#!/usr/bin/perl

my $arg = shift;
my $buf = `man 2 $arg`;
if(length($buf) == 0) {
	$buf = `man 3 $arg`;
}
open(FH, "> $ENV{HOME}/.vim/mmm") or die;
print FH $buf;

close(FH);
```

![light](https://github.com/dogdio/fbt/blob/for_img/img/light.png "light") **出力結果の取り込み**<br>
出力結果を取り込むときは、`:tabnew` してから `:r` で読み込むのがお手軽だ。vimでTMPファイルを開かないことでバッティングを防げる（連続で実行可能となる）。TMPファイルの名前は常に同じでも問題ない（乱数や連番でファイル名を毎回変える必要がない）。

★ 拡張子の関連付け<br>
`*.txt` のファイルを開いたら、filetype=txt として解釈する<br>
`~/.vim/syntax/txt.vim` を作成すれば、syntaxを独自に定義できる<br>
```
:au BufRead,BufNewFile *.conf set filetype=conf
:au BufRead,BufNewFile *.txt set filetype=txt
```

★ ステータス<br>
ステータスラインを常に表示する(=2)。0は常に表示しない。1だとウィンドウが複数の場合のみ
```
set laststatus=2
```

★ 256色対応<br>
colorscheme で設定した名前と同じファイル [~/.vim/colors/256_hoge.vim](https://github.com/dogdio/fbt/blob/master/vim/colors/256_hoge.vim) を用意しておく。
カラー設定は詳細に決められるので、好きなように記載する。
```
set t_Co=256
colorscheme 256_hoge
```
`/usr/share/vim/XXX/syntax` の配下に、大量の `***.vim` ファイルがある。
ここで各プログラム言語で使用されているSyntaxの設定がある。
これを `.vim/` 配下のファイルでオーバーライドできる。パラメータの名前はこのファイルから探せばOK。

★ コメント中の特定文字列だけ別の色にする方法<br>
システム標準の設定ファイルを個人ディレクトリにコピーして、適宜編集する
```
$ cp /usr/share/vim/vim82/syntax/c.vim ~/.vim/syntax/c.vim
$ vi c.vim
```
cCommentGroup に新たに定義したルールを追加する（TODO, FIXME などと同じ扱いで色だけ変える）。<br>
以下の例では、doxygen の文字列を別の色 (CommentDox) にしている
```vim
syn match   cDox       contained "@[a-z]*"
syn match   cDox2      contained "\!<"
syn cluster    cCommentGroup   contains=cTodo,cBadContinuation,cDox,cDox2
hi def link cDox               CommentDox
hi def link cDox2              CommentDox
```

以下を、 [~/.vim/colors/256_hoge.vim](https://github.com/dogdio/fbt/blob/master/vim/colors/256_hoge.vim) に書いておく。
```vim
hi CommentDox        ctermfg=58
```

★ 折りたたみ<br>
巨大な関数があり全体を把握しにくいときは、折りたたみを使う<br>
インデント単位で折りたたむ場合は、`foldmethod=indent` とする<br>
折りたたみを開くのが `zo`, 閉じる場合は `zc`, 全て開く場合は `zR`, 全て閉じるときは`zM` <br>
```
:set foldmethod=indent
```

★ c++ のラムダ式を構文エラーと見なさない
```
let c_no_curly_error=1
```

★ C++に対応したctags<br>
https://github.com/universal-ctags/ctags
