#!/bin/bash
TMP=.tmp.txt
rm -f $TMP

if [ $# -eq 0 ]; then
	args=(".")
else
	args=$@
fi

for dir in "${args[@]}"
do
	find $dir -name "*.[ch]"    >> $TMP
	find $dir -name "*.cpp"     >> $TMP
	find $dir -name "*.java"    >> $TMP
	find $dir -iname "makefile" >> $TMP
done

cat $TMP |sort > all.txt
rm -f $TMP
