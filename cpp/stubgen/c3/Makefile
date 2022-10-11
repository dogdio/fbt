SHELL = /bin/bash
CC    = gcc
CXX   = g++
LD    = g++
MAKE  = make

WARNING   = -Wall -Wno-unused-value -Wno-unused
CFLAGS1  ?= $(WARNING) -O3
FILES1    = $(shell find *.c)
TARGETS   = hoge hoge_ex

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS1) $^ -o $@

func.txt: main.o
	ln -sf /usr/include/openssl/evp.h .
	../stubgen.pl -w func.txt main.o

hoge_ex.c: func.txt
	../stubgen.pl -r func.txt -g hoge_ex.c

hoge_ex.o: hoge_ex.c
	$(CC) -c $^

hoge: main.o
	$(CC) $^ -o $@ -lcrypto

hoge_ex: main.o hoge_ex.o
	$(CC) $^ -o $@

clean:
	rm -f $(TARGETS) *.o func.txt hoge_ex.c evp.h

