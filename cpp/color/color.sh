#!/bin/bash

for i in {0..255}; do
    printf "\x1b[38;5;${i}mcolor${i}\x1b[0m "
    if [ $((i % 10)) -eq 0 ]; then
        printf "\n"
    fi
done
echo

