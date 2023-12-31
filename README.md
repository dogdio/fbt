# fbt
(for basic training/tools)

## environment

### on WSL2
```
$ uname -a
Linux corvus-wsl 5.15.133.1-microsoft-standard-WSL2 #1 SMP Thu Oct 5 21:02:42 UTC 2023 x86_64 GNU/Linux

$ g++ --version
g++ (Debian 12.2.0-14) 12.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ java --version
openjdk 17.0.6 2023-01-17
OpenJDK Runtime Environment (build 17.0.6+10-Debian-1)
OpenJDK 64-Bit Server VM (build 17.0.6+10-Debian-1, mixed mode, sharing)
```

### on VirtualBox 7.0.12

```
$ uname -a
Linux corvus-deb12 6.1.0-16-amd64 #1 SMP PREEMPT_DYNAMIC Debian 6.1.67-1 (2023-12-12) x86_64 GNU/Linux

$ g++ --version
g++ (Debian 12.2.0-14) 12.2.0
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ java --version
openjdk 17.0.9 2023-10-17
OpenJDK Runtime Environment (build 17.0.9+9-Debian-1deb12u1)
OpenJDK 64-Bit Server VM (build 17.0.9+9-Debian-1deb12u1, mixed mode, sharing)
```

### WSLとVirtualBoxとの共存について

Windows11 Home(22H2)で可能だった。以下ですんなりと動いた。<br>

「Windowsの機能の有効化または無効化」を開く
```
Windowsキー + r
optionalfeatures + Enter
```

以下を全部ONにする
- Linux用Windowsサブシステム
- Windowsハイパーバイザープラットフォーム
- 仮想マシンプラットフォーム

### samba
VirtualBoxと通信するのにあったほうがいい(Windows11はデフォルトでOFF)
- SMB 1.0/CIFS ファイル共有のサポート
（配下も全部チェックする）

### 再配布パッケージ
VirtualBox 7以上をインストールする際に再配布パッケージが必要<br>
https://learn.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist?view=msvc-170#visual-studio-2015-2017-2019-and-2022

### VirtualBox 設定

GuestAddition CD イメージの挿入(メニューから実施)
```
$ sudo mount /dev/cdrom /mnt/cdrom/
$ sudo ./VBoxLinuxAdditions.run
→ VMを再起動する
```

ネットワーク設定は「ブリッジ」が安パイ。もう一台物理的に増えるような感じになる。

