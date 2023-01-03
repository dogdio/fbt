#!/bin/sh

#rm -f hoge.info
rm -rf ./aaa/
mkdir -p aaa

#lcov -c -d ./target/.objs -o hoge.info
#lcov -r hoge.info "/usr*" -o hoge.info
#genhtml -o aaa hoge.info

gcovr --exclude-throw-branches --exclude-unreachable-branches \
      --html-details -o ./aaa/coverage.html -s
