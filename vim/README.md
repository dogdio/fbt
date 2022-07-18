
## vim

[vim���{��h�L�������g](https://vim-jp.org/vimdoc-ja/)


### Concept
�����N�����f�[�^��ex�R�}���h��g�ݍ��킹�āA���͂��������������̂�map�Ŋ��蓖�Ă�<br>
�g�p�p�x�������A���p���l�̍����R�}���h�� .vimrc �� map ���Ă���

���J�[�\���ʒu�̕�������擾<br>
���݂̃J�[�\���ʒu�̕�����i�P��j�������N<br>
```
yiw
```

ex�R�}���h�ŁA`Ctrl+r`�̌�ɁA`�h`�����
�i���݂̃J�[�\���ʒu�̕�������R�s�[���āA�A�A������ex�R�}���h�ŉ������s���j
```
:<C-r>�h
```

��������������̎擾
```
:<C-r>/
```

��L�𗘗p���āA�V�K�R�}���h���쐬�i�����N�����f�[�^�̎擾 + ex�R�}���h�j

<br>

�� ��ʂ̃R�[�h���ǂ�����ēǂݏ������邩�H�iall.txt �j
���炩���ߎ����̌���ׂ��t�@�C�������X�g�A�b�v���Ă���<br>
���̃��X�g���ꂽ�t�@�C��(all.txt)�o�R�ŁA�������t�@�C��(*.c, *.h)���J��<br>
�^�[�~�i������ `vim xx/yy/zz.c` �ȂǂƂ͂����������Ȃ��I<br>
���̃��X�g�����ꂽ�t�@�C���̈ꗗ��tag-jump, grep �̑ΏۂƂ���iIDE�̃��[�N�X�y�[�X�j<br>
�c��ȃf�B���N�g���Q���猩��ׂ��Ώۂ��i�荞�݁A�ꗗ�𐶐�����B[�������@](https://github.com/dogdio/fbt/blob/master/vim/bin/All.sh)�͉��ł�OK�B<br>
�f�B���N�g�����������ꍇ�́A�t�H���_���@���āA���̒�����V���{���b�N�����N���g��<br>
�����̒S���͈͂Ƌ��ʃ��C�u�����A���ʑw�A��ʑw��������ꗗ�ɂ��Ă���<br>
```
$ find dir1/ -name �g*.[ch]�h  > all.txt
$ find dir2/ -name �g*.cpp�h  >> all.txt
```

```
�֌W����R�[�h�͑S��all.txt�ɋL�ڂ��A�{�^���ꔭ��grep, tag-jump�ł���悤�ɂ���
  [UpperLayer]
       ��
  [  My Code ] <--> [Common Library]
       ��
  [UnderLayer]
```

all.txt���J���āA�c���� `vs` ���āA�����Ɉꗗ��\��<br>
�������J�������t�@�C����vim��Ō�������B`/file<CR>` �Ƃ��邱�ƂŁA�ڕW���ɑf�������ǂ蒅���B<br>
�t�@�C���u���E�U�A�R���\�[����ʂ���t�@�C����T���K�v�͖����B<br>
�J�[�\�������ɂ����āA`Ctrl+f`�Ńt�@�C�����E���ɊJ���i�����ɂ͈ꗗ���L�[�v�����j
�����ɊJ���ꍇ�́A`Shift+f` �Ahoge.c �����Ă���Ƃ��ɁAall.txt�ɖ߂�Ƃ��́A`Shift-A`
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



�u�����Ɉꗗ�A�E���ɃR�[�h�v�A�̂悤�ɕ��ׂĕ\������ƌ��₷���Ȃ�<br>
grep���ʂ͉E���̉������ɕ\�������i�ڍׂ͌�q�j<br>
�����Ń������Ƃ�A�E���ɃR�[�h��\������Ƃ������Ƃ��ł���<br>
all.txt �ɐV�����t�@�C������ǋL���āA`Ctrl+f`�ŐV�K�t�@�C���Ƃ��ăI�[�v���ł���<br>
