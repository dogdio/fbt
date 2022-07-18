#!/bin/sh
cat tags | awk '{print $1}' | grep  $1 | uniq -c > ~/.vim/hoge.tmp

