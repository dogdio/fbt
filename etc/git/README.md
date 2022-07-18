
## git/ssh �R�}���h

���U�^�̓���T�v<br>
push���Ȃ�����̓����[�g�ɂ͔��f����Ȃ��̂Ń��[�J���ł�����ł���蒼����<br>
commit(+push)�O��pull���ă��[�J�����ŐV�ɂ��Ă����Ȃ��ƁA���Ԃ����������Ȃ����Ⴄ�̂Œ���
```
                    commit              push
Working Directory ---------> [Local] =========> [Remote]
                                     <=========
                                        pull
```

<br>

��{�ݒ�B���[�U�A���[���̓o�^�Apull�ݒ�A�G�f�B�^
```
$ git config --global user.name  hogehoge
$ git config --global user.email hoge@hoge.com
$ git config pull.rebase false
$ git config --global core.editor vim
```

SSH
�J�M�̐���
```
$ ssh-keygen -t rsa
�p�X�t���[�Y���� *1
```

���J��
```
$ cat ~/.ssh/id_rsa.pub
--> �����\��t����
```

Settings > SSH and GPG keys �� SSH keys�� "New SSH key" �Ƃ���
[�΂̃{�^��](https://github.com/settings/ssh/new) ��������ăR�s�y����


�p�X���͂̎�����
```
$ ssh-agent bash
$ ssh-add #�p�X����� *1
--> ���̏�Ԃ��� git *** �����{����ƃp�X���͂��Ȃ��Ă���
```

<br>

���|�W�g���擾
```
$ git clone git@github.com:dogdio/fbt.git
�i���O�Ƀp�X���͂����������Ă����j
```

�V�����u�����`�̍쐬<br>
hoge�u�����`���N�_�ɂ��ĐV�����u�����`�̍쐬
```
$ git checkout -b ***
$ git checkout -b *** hoge
```

�u�����`�̐؂�ւ�
```
$ git checkout my_branch
```

�C����S�����ɖ߂�
```
$ git checkout .
```

����̃t�@�C�������ɖ߂�
```
$ git checkout hoge.c
```

�ύX����荞��(�f�t�H���g�̓R�~�b�g�܂ōs��)<br>
���O�œ��e���m�F�\
```
$ git cherry-pick commit-id
$ git log
```

�ύX����荞�ނ��R�~�b�g�͂��Ȃ�(diff��������)
```
$ git cherry-pick commit-id -n
$ git diff
```

<br>

���[�J���u�����`���m�F<br>
�u�����`�ꗗ������
```
$ git branch
$ git branch -a 
```

add���Ă���commit����<br>
add����Ă��Ȃ����̂̓R�~�b�g����Ȃ��Ŗ��������
```
$ git add hoge.c
$ git status # �ǉ��ꗗ�m�F����(�ΐF�ɂȂ��Ă���)
$ git commit -m "hoge hoge"
```

commit�����烍�O��������悤�ɂȂ�<br>
--stat �ŏC���t�@�C������������
```
$ git log
$ git log --stat
```

���O���C���������Ƃ��iPUSH�O�Ɏg���j
```
$ git commit -m "hoge hoge hoge" --amend
```

�v�b�V������
```
$ git push origin master
$ git push origin my_branch
```

�C���̂������t�@�C����\��(-m)<br>
�C���̂������t�@�C�����܂Ƃ߂āAgit add<br>
git�Ǘ��O�̃t�@�C����\��(-o)
```
$ git ls-files -m
$ git ls-files -m | xargs git add
$ git ls-files -o
```

<br>

���O�̃R�~�b�g�̎�����
```
$ git reset --soft HEAD^
```

�����̊m�F(git add �̑O�Ɏg��)
```
$ git diff
$ git diff -w  #�󔒂𖳎�
```

add��������
```
$ git reset
```

ID1��ID2�̍���
```
$ git diff ID1..ID2
```


