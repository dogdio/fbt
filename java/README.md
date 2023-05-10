# Java���낢��

## maven

�v���W�F�N�g�Ǘ��c�[��  
�R�}���h�� `mvn` ���g��  
https://ja.wikipedia.org/wiki/Apache_Maven

> ���̃c�[���̑傫�ȓ����̓v���O�C���g���ɂ��l�X�Ȏg�������ł��邱�Ƃł���B�\�[�X�R�[�h�̃R���p�C���A�e�X�g�AJavadoc�����A�e�X�g���|�[�g�����A�v���W�F�N�g�T�C�g�����AJAR�����A�T�[�o�ւ̃f�v���C�AWAR, EAR�t�@�C�������ȂǗl�X�ȋ@�\���p�ӂ���Ă���AAnt�̏ꍇ�ɂ�build.xml�Ƃ����ݒ�t�@�C���ɍׂ����w�����L�q���čs���Ă����e�������AMaven�ł͑�܂��Ȏw����pom.xml�ɋL�q���ď�������`�ƂȂ��Ă���B

## jacoco

Java Code Coverage Library  
https://www.eclemma.org/jacoco/  
> JaCoCo is a free code coverage library for Java, which has been created by the EclEmma team based on the lessons learned from using and integration existing libraries for many years.

pom.xml �̐ݒ�  
https://www.jacoco.org/jacoco/trunk/doc/maven.html  
xml�̋L�q�͂���Ɠ�������Ȃ��Ă�������B�B�B
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


jacoco��version�ɂ���ẮAjava calss file �̑Ή�version���قȂ�̂Œ���  
javac�Ő�������class file�ƈ�v���Ȃ��ƃ_��(�ȉ��ɂ������Ə����Ă���)�B  
https://www.jacoco.org/jacoco/trunk/doc/changes.html  
java��version��pom.xml�Ŏw��ł���

### mvn, jacoco �C���X�g�[�����@  

mvn �����|�W�g�����炢�낢����������Ă���̂ŁAjacoco �̓C���X�g�[�����Ȃ��Ă����v�����B�B

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

### mvn �g����

1. �W���̃t�H���_�\�������(HelloWorld��p�ӂ���)
2. pom.xml ���L�q����

�e�X�g�����̍ŏ��\��(sample.Hoge)  
pom.xml �� *.java ��������OK

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
./src/main/java/sample/Hoge.java <-- main�֐���HelloWorld���������OK
./src/main/resources
```

mvn �R�}���h

```
�Ƃ肠��������2��OK
$ mvn clean
$ mvn package
*.jar �� maven-archiver �� target/ �z���ɐ��������
�� �e�X�g�̂����͌�q
```

### jacoco �g����  

`mvn package` �����Ƃ��ɕ\�������A�ȉ��̕����ɕ\�������I�v�V���� `-javaagent:/***` ���R�s�y����java���N������  
���̃I�v�V������ǉ����Ȃ��ƁAjacoco.exec�Ƀf�[�^���~�ς���Ȃ�  
clean ���Ȃ�����f�[�^�͂ǂ�ǂ�~�ς����i�e�X�g����傫���ς����ꍇ�� clean �����ق��������j

```
[INFO] --- jacoco-maven-plugin:0.8.6:prepare-agent (default) @ sample-maven ---
[INFO] argLine set to -javaagent:/home/***/***/....
```

```
sample.Main ���N�������(���jacoco�����A����jacoco�L��)
$ java -cp target/sample-1.0.0.jar sample.TestHoge
$ java -javaagent:/home/***/.m2/repository/org/jacoco/org.jacoco.agent/0.8.6/org.jacoco.agent-0.8.6-runtime.jar=destfile=/home/***/tmp/fbt/java/sample-maven/target/jacoco.exec -cp target/sample-1.0.0.jar sample.TestHoge

���m���I������炱��ŏo�͂���
$ mvn jacoco:report

���s�f�[�^�t�@�C���i�����ɋL�^�����j
$ ls target/jacoco.exec
target/jacoco.exec

���̉��ɃJ�o���b�W�̌��ʂ������Ă���
$ ls target/site/jacoco/
index.html  jacoco-resources/  jacoco-sessions.html  jacoco.csv  jacoco.xml  sample/
```

## Junit

Java�p�̒P�̎����̃t���[�����[�N  
https://junit.org/junit4/  
https://junit.org/junit5/  

junit �����ł�����������ۂ����Amaven/jacoco �ȂǂƘA�����Ď����Ɠ����ɃJ�o���b�W���v���ł����ق�������  
�ijunit �����ł̓J�o���b�W�͌v���ł��Ȃ��j  

### maven/jacoco �Ƃ̘A�g

[Maven Surefire Plugin](https://maven.apache.org/surefire/maven-surefire-plugin/)  
Maven �v���W�F�N�g�̃e�X�g�����s���邽�߂̃v���O�C��  
���̃v���O�C���́AJUnit �� TestNG �Ȃǂ̃e�X�g�����i�[���g�p���ăe�X�g�������I�Ɏ��s���A�e�X�g���ʂ��o�͂���  

pom.xml �̃v���O�C���̕����̋L�ڗ�  
`argLine` �̕����́Ajava �ɂ킽��jacoco�̃I�v�V�����ƂȂ�imvn package�ŏo�͂������̂Ɠ����j  

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

pom.xml �Ɉˑ��֌W�Ƃ��� junit ���L�ڂ���  
`version` ������Ȃ��ꍇ�́A�r���h���ɓ{����̂ŁA[���|�W�g��](https://repo.maven.apache.org/maven2/) ����T��

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

### Test�R�[�h

assert API���g�����́Aimport static �ɂ��Ȃ��ƃr���h�ł��Ȃ��̂Œ���  
junit5 ���Ə��������ς����ۂ��i�ȉ��́Ajunit4�p�̃R�[�h�j  
`@Test` �����Ă��Ȃ����\�b�h�͎��s����Ȃ�

test�z���Ɉȉ��̗p�ȃR�[�h�����Ă���  
src/test/java/sample/TestHoge.java  
```Java
// junit4
import org.junit.Test; // Test�A�m�e�[�V�����ɑΉ�����
import static org.junit.Assert.*; // static�����o�̃N���X�����ȗ��ł���

public class TestHoge {

	@Test // �e�X�g�Ώۂ̃��\�b�h���w�肷��
	public void test_is_zero()
	{
		Hoge h = new Hoge();
		assertTrue(h.is_zero(0));
		assertFalse(h.is_zero(1));
		System.out.println("zero");
	}
```
### Test���{

�r���h�����炻�̂܂܃e�X�g���s�A���|�[�g�����܂ł���Ă����
```
���ꂾ����OK
$ mvn clean
$ mvn test

����̎������������{����ꍇ
$ mvn test -Dtest=ClassName#testMethodName

src/main �̃r���h�������s��
$ mvn compile

src/main, src/test �̃r���h�������s��
$ mvn test-compile
```


