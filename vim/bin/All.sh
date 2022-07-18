#!/bin/bash

TMP=.tmp.txt
find -name "*.[ch]" > $TMP
find -name "*.cpp" >> $TMP
find -name "*.java" >> $TMP
find -iname "makefile" >> $TMP
cat $TMP |sort > all.txt
rm -f $TMP
