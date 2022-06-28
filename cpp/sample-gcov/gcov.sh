#!/bin/sh

rm -f hoge.info
rm -rf ./aaa/
lcov -c -d .objs -o hoge.info
genhtml -o aaa hoge.info 

