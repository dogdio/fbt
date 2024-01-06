# Javaいろいろ

## maven

プロジェクト管理ツール  
コマンドは `mvn` を使う  
https://ja.wikipedia.org/wiki/Apache_Maven

> このツールの大きな特徴はプラグイン拡張により様々な使い方ができることである。ソースコードのコンパイル、テスト、Javadoc生成、テストレポート生成、プロジェクトサイト生成、JAR生成、サーバへのデプロイ、WAR, EARファイル生成など様々な機能が用意されており、Antの場合にはbuild.xmlという設定ファイルに細かい指示を記述して行っていた各処理を、Mavenでは大まかな指示をpom.xmlに記述して処理する形となっている。

## jacoco

Java Code Coverage Library  
https://www.eclemma.org/jacoco/  
> JaCoCo is a free code coverage library for Java, which has been created by the EclEmma team based on the lessons learned from using and integration existing libraries for many years.

pom.xml の設定  
https://www.jacoco.org/jacoco/trunk/doc/maven.html  
xmlの記述はこれと同じじゃなくてもいける。。。
```xml
<plugin>
  <groupId>org.jacoco</groupId>
  <artifactId>jacoco-maven-plugin</artifactId>
  <version>0.8.6</version>
  <executions>
    <execution>
      <goals>
        <goal>prepare-agent</goal>
      </goals>
    </execution>
    <execution>
      <id>report</id>
      <phase>test</phase>
      <goals>
        <goal>report</goal>
      </goals>
    </execution>
  </executions>
</plugin>
```


jacocoのversionによっては、java calss file の対応versionが異なるので注意  
javacで生成するclass fileと一致しないとダメ(以下にちょろっと書いてある)。  
https://www.jacoco.org/jacoco/trunk/doc/changes.html  
javaのversionはpom.xmlで指定できる

### mvn, jacoco インストール方法  

mvn がリポジトリからいろいろ引っ張ってくるので、jacoco はインストールしなくても大丈夫かも。。

```
$ apt-get install maven
$ apt-get install libjacoco-java

$ mvn -v
Apache Maven 3.6.3
Maven home: /usr/share/maven
Java version: 17.0.3, vendor: Debian, runtime: /usr/lib/jvm/java-17-openjdk-amd64
Default locale: ja_JP, platform encoding: UTF-8
OS name: "linux", version: "5.10.0-10-amd64", arch: "amd64", family: "unix"
```

### mvn 使い方

1. 標準のフォルダ構成を作る(HelloWorldを用意する)
2. pom.xml を記述する

テスト無しの最小構成(sample.Hoge)  
pom.xml と *.java が一つあればOK

```
$ find
./pom.xml
./src
./src/test
./src/test/java
./src/test/resources
./src/main
./src/main/java
./src/main/java/sample
./src/main/java/sample/Hoge.java <-- main関数でHelloWorld実装すればOK
./src/main/resources
```

mvn コマンド

```
とりあえずこの2つでOK
$ mvn clean
$ mvn package
*.jar と maven-archiver が target/ 配下に生成される
※ テストのやり方は後述
```

### jacoco 使い方  

`mvn package` したときに表示される、以下の部分に表示されるオプション `-javaagent:/***` をコピペしてjavaを起動する  
このオプションを追加しないと、jacoco.execにデータが蓄積されない  
clean しない限りデータはどんどん蓄積される（テスト側を大きく変えた場合は clean したほうがいい）

```
[INFO] --- jacoco-maven-plugin:0.8.6:prepare-agent (default) @ sample-maven ---
[INFO] argLine set to -javaagent:/home/***/***/....
```

```
sample.Main を起動する例(上はjacoco無し、下はjacoco有り)
$ java -cp target/sample-1.0.0.jar sample.TestHoge
$ java -javaagent:/home/***/.m2/repository/org/jacoco/org.jacoco.agent/0.8.6/org.jacoco.agent-0.8.6-runtime.jar=destfile=/home/***/tmp/fbt/java/sample-maven/target/jacoco.exec -cp target/sample-1.0.0.jar sample.TestHoge

同確が終わったらこれで出力する
$ mvn jacoco:report

実行データファイル（ここに記録される）
$ ls target/jacoco.exec
target/jacoco.exec

この下にカバレッジの結果が入っている
$ ls target/site/jacoco/
index.html  jacoco-resources/  jacoco-sessions.html  jacoco.csv  jacoco.xml  sample/
```

## Junit

Java用の単体試験のフレームワーク  
https://junit.org/junit4/  
https://junit.org/junit5/  

junit だけでも動かせるっぽいが、maven/jacoco などと連動して試験と同時にカバレッジを計測できたほうがいい  
（junit だけではカバレッジは計測できない）  

### maven/jacoco との連携

[Maven Surefire Plugin](https://maven.apache.org/surefire/maven-surefire-plugin/)  
Maven プロジェクトのテストを実行するためのプラグイン  
このプラグインは、JUnit や TestNG などのテストランナーを使用してテストを自動的に実行し、テスト結果を出力する  

pom.xml のプラグインの部分の記載例  
`argLine` の部分は、java にわたすjacocoのオプションとなる（mvn packageで出力されるものと同じ）  

```xml
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-surefire-plugin</artifactId>
  <version>3.0.0</version>
  <configuration>
    <argLine>-javaagent:${settings.localRepository}/org/jacoco/org.jacoco.agent/0.8.6/org.jacoco.agent-0.8.6-runtime.jar=destfile=target/jacoco.exec</argLine>
  </configuration>
</plugin>
```

pom.xml に依存関係として junit を記載する  
`version` が合わない場合は、ビルド時に怒られるので、[リポジトリ](https://repo.maven.apache.org/maven2/) から探す

```xml
<dependencies>
  <dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <version>4.13.1</version>
    <scope>test</scope>
  </dependency>
</dependencies>
```

### Testコード

assert APIを使う時は、import static にしないとビルドできないので注意  
junit5 だと書き方が変わるっぽい（以下は、junit4用のコード）  
`@Test` がついていないメソッドは実行されない

test配下に以下の用なコードを入れておく  
src/test/java/sample/TestHoge.java  
```Java
// junit4
import org.junit.Test; // Testアノテーションに対応する
import static org.junit.Assert.*; // staticメンバのクラス名を省略できる

public class TestHoge {

	@Test // テスト対象のメソッドを指定する
	public void test_is_zero()
	{
		Hoge h = new Hoge();
		assertTrue(h.is_zero(0));
		assertFalse(h.is_zero(1));
		System.out.println("zero");
	}
```
### Test実施

ビルドしたらそのままテスト実行、レポート生成までやってくれる
```
これだけでOK
$ mvn clean
$ mvn test

特定の試験だけを実施する場合
$ mvn test -Dtest=ClassName#testMethodName

src/main のビルドだけを行う
$ mvn compile

src/main, src/test のビルドだけを行う
$ mvn test-compile
```


