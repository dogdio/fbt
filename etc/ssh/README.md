
## sshd のインストール
```
$ sudo apt-get install openssh-server
```

## 鍵の運用
準備

```
Client側で、鍵を生成して公開鍵をサーバに送る
$ ssh-keygen -t rsa

  [Client] --------------------- [Server]
  1.鍵生成                       2.公開鍵 転送
    id_rsa(秘密鍵)                 ~/.ssh/authorized_keys に id_rsa.pub の内容を追記
    id_rsa.pub(公開鍵)

```

## ログインの自動化

### Linux
```
$ ssh-agent bash
$ ssh-add          # 秘密鍵のパスを入力
→ この後、パスは入力しなくてもOKとなる

$ ssh user@your.server.com
$ scp src_file user@your.server.com:/home/user/dst  # Client --> Server
$ scp -r user@your.server.com:/home/user/src ./dst  # Server --> Client
```
サーバーとクライアントのユーザー名が同じなら `user` は省略可能<br>
scp では、`-r` をつけると配下のフォルダを全部コピーできる

### Windows
Teratermマクロを用意する(HOST, USER等は適宜変更する)<br>
★auto-login.ttl
```
HOSTADDR     = '192.168.0.1'
USERNAME     = 'user'
KEYFILE      = 'C:\xxx\yyy\id_rsa'
PASSWORDFILE = 'C:\xxx\yyy\pass.dat'

getpassword PASSWORDFILE USERNAME PASSWORD

COMMAND = HOSTADDR
strconcat COMMAND ':22 /ssh /2 /auth=publickey /user='
strconcat COMMAND USERNAME
strconcat COMMAND ' /passwd='
strconcat COMMAND PASSWORD
strconcat COMMAND ' /keyfile='
strconcat COMMAND KEYFILE

connect COMMAND

end
```

auto-login.ttl をダブルクリックすればOK<br>
初回のみパスワードを聞いてくるので入力する。初回入力後は暗号化されて保持されるので省略できる。


### sshd 設定(/etc/ssh/sshd_config)
以下で、パスワード認証できなくなる(鍵の準備が整ってから）。
```
PasswordAuthentication no
```

