CXX        = $(CPU)g++
CC         = $(CPU)gcc
AR         = $(CPU)ar
LD         = $(CPU)g++

export CC AR LD CXX

SUBDIR_ALL     =  test plugin plugin2

all:
	for n in $(SUBDIR_ALL); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIR_ALL); do $(MAKE) clean -C $$n || exit 1; done

doxpub:
	doxygen doxygen-pub.conf

doxpriv:
	doxygen doxygen-priv.conf

