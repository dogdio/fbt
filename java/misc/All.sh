#!/bin/bash

find -name "*.java" | sort > all.txt
find -iname makefile | sort >> all.txt
