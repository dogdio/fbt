
## git/ssh コマンド

分散型の動作概要<br>
pushしない限りはリモートには反映されないのでローカルでいくらでもやり直せる<br>
commit(+push)前にpullしてローカルを最新にしておかないと、順番がおかしくなっちゃうので注意
```
                    commit              push
Working Directory ---------> [Local] =========> [Remote]
                                     <=========
                                        pull
```

<br>

基本設定。ユーザ、メールの登録、pull設定、エディタ
```
$ git config --global user.name  hogehoge
$ git config --global user.email hoge@hoge.com
$ git config pull.rebase false
$ git config --global core.editor vim
```

SSH
カギの生成
```
$ ssh-keygen -t rsa
パスフレーズ入力 *1
```

公開鍵
```
$ cat ~/.ssh/id_rsa.pub
--> これを貼り付ける
```

Settings > SSH and GPG keys の SSH keysの "New SSH key" という
[緑のボタン](https://github.com/settings/ssh/new) から入ってコピペする<br>
鍵は途中でいつでも変更可能（cloneしてから鍵を変えて、pull とかもOK）

パス入力の自動化
```
$ ssh-agent bash
$ ssh-add #パスを入力 *1
--> この状態から git *** を実施するとパス入力しなくておｋ
```

<br>

リポジトリ取得
```
$ git clone git@github.com:dogdio/fbt.git
（事前にパス入力を自動化しておく）
```

新しいブランチの作成<br>
hogeブランチを起点にして新しいブランチの作成
```
$ git checkout -b ***
$ git checkout -b *** hoge
```

ブランチの切り替え
```
$ git checkout my_branch
```

修正を全部元に戻す
```
$ git checkout .
```

特定のファイルを元に戻す
```
$ git checkout hoge.c
```

変更を取り込む(デフォルトはコミットまで行う)<br>
ログで内容を確認可能
```
$ git cherry-pick commit-id
$ git log
```

変更を取り込むがコミットはしない(diffが見える)
```
$ git cherry-pick commit-id -n
$ git diff
```

<br>

ローカルブランチを確認<br>
ブランチ一覧を見る
```
$ git branch
$ git branch -a 
```

addしてからcommitする<br>
addされていないものはコミットされないで無視される
```
$ git add hoge.c
$ git status # 追加一覧確認する(緑色になっている)
$ git commit -m "hoge hoge"
```

commitしたらログが見えるようになる<br>
--stat で修正ファイル名が見える
```
$ git log
$ git log --stat
```

ログを修正したいとき（PUSH前に使う）
```
$ git commit -m "hoge hoge hoge" --amend
```

プッシュする
```
$ git push origin master
$ git push origin my_branch
```

修正のあったファイルを表示(-m)<br>
修正のあったファイルをまとめて、git add<br>
git管理外のファイルを表示(-o)
```
$ git ls-files -m
$ git ls-files -m | xargs git add
$ git ls-files -o
```

<br>

直前のコミットの取り消し
```
$ git reset --soft HEAD^
```

差分の確認(git add の前に使う)
```
$ git diff
$ git diff -w  #空白を無視
```

addを取り消す
```
$ git reset
```

ID1とID2の差分
```
$ git diff ID1..ID2
```


