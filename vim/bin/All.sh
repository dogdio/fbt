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
	find $dir -name "*.dot"     >> $TMP
	find $dir -name "*.msc"     >> $TMP
	find $dir -name "*.conf"    >> $TMP
	find $dir -name "*.java"    >> $TMP
	find $dir -name "*.sh"      >> $TMP
	find $dir -name "*.pl"      >> $TMP
	find $dir -name "*.js"      >> $TMP
	find $dir -name "*.json"    >> $TMP
	find $dir -name "*.html"    >> $TMP
	find $dir -name "*.xml"    >> $TMP
	find $dir -name "*.css"     >> $TMP
	find $dir -iname "makefile" >> $TMP
	find $dir -name "*.gradle"        >> $TMP
	find $dir -name "*.properties"    >> $TMP
done

cat $TMP |sort > all.txt
rm -f $TMP
