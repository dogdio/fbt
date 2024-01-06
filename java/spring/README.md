
## Spring のデモアプリ
https://start.spring.io/

上記のInitializrを使って生成したサンプル(maven/gradleの両方でお試し)
- demo
- demo-js (JavaScript確認用)
- demo-th (Thymeleaf確認用)
- demo-app (ToDo管理アプリ)

## ビルドツールの使い方
https://spring.io/guides/gs/spring-boot/

maven
```
./mvnw clean
./mvnw compile
./mvnw test
./mvnw spring-boot:run
```

gradle
```
./gradlew clean
./gradlew build
./gradlew test
./gradlew bootRun

テスト結果はこの辺にある
./build/test-results/test/
```


## demo-app の使い方
Webアプリの学習用に作ったシンプルなTODO管理アプリ<br>

### postgres pass設定 

```
$ sudo -u postgres psql
psql (15.5 (Debian 15.5-0+deb12u1))
Type "help" for help.

postgres=# \password postgres
Enter new password for user "postgres":
Enter it again:
```

設定ファイル変更
```
$ sudo vi /etc/postgresql/15/main/pg_hba.conf
local   all             postgres                                password  <---- 平文パスワード認証
```

これでいちおうpsql起動時の入力を省ける
```
$ export PGPASSWORD=***
```

### spring CLI のインストール
https://spring.pleiades.io/spring-boot/docs/current/reference/html/cli.html#cli.using-the-cli<br>
パスワードエンコード等のツールがある。ダウンロードして適当なフォルダに展開しておく

```
$ tar xzvf spring-boot-cli-3.2.1-bin.tar.gz
$ cd spring-3.2.1
$ ln -sf ./shell-completion/bash/spring /etc/bash_completion.d/spring
$ ln -sf /home/user/tmp/spring-3.2.1/bin/spring /home/user/bin/spring
$ export PATH=$PATH:/home/user/bin
```

DB作成（最初に1回だけやる)
```
$ psql -U postgres
$ create database demodb;
```

テーブル初期化
```
$./psql.sh psql_init.txt
```

admin作成(spring CLIが必須)
```
$ ./create_admin.sh
→ これで adminでログインできるようになる
```

### webアプリ起動
```
$ ./gradlew bootRun
Starting a Gradle Daemon (subsequent builds will be faster)

> Task :bootRun

  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::                (v3.0.6)

```

DBのバックアップと復旧<br>
復旧前にテーブルなどは全部消しておいた方がよい
```
$ pg_dump -U <ユーザー名> -h <ホスト名> -d <データベース名> > backup.sql
$ psql -U <ユーザー名> -h <ホスト名> -d <データベース名> -f backup.sql
```


