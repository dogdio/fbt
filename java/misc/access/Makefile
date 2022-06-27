SHELL = /bin/bash
JAVAC = javac
MAKE  = make

FILES1    = $(shell find -name "*.java")
TARGETS   = $(FILES1:.java=.class)

all: $(TARGETS)

%.class: %.java
	@echo "  [JAVAC] $^"
	@$(JAVAC) $(JCFLAGS1) $^

clean:
	rm -f $(TARGETS)


