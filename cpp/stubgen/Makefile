CXX        = $(CPU)g++
CC         = $(CPU)gcc
AR         = $(CPU)ar
LD         = $(CPU)g++

export CC AR LD CXX

SUBDIR_ALL     =  cpp1 cpp2 cpp3 cpp4 c1 c2 c3

all:
	for n in $(SUBDIR_ALL); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIR_ALL); do $(MAKE) clean -C $$n || exit 1; done

