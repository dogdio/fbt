
SUBDIR_ALL     =  $(shell find -maxdepth 1 -type d | sed s/\.// | sed s/\\///)

all:
	for n in $(SUBDIR_ALL); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIR_ALL); do $(MAKE) clean -C $$n || exit 1; done

hoge:
	echo $(SUBDIR_ALL)

