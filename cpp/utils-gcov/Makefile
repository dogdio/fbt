#-include config.mk

CXX        = $(CPU)g++
AR         = $(CPU)ar
LD         = $(CPU)g++

export CXX AR LD

SUBDIR_ALL = target test

all:
	for n in $(SUBDIR_ALL); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIR_ALL); do $(MAKE) clean -C $$n || exit 1; done

