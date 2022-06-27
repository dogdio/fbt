SHELL = /bin/bash
JAVAC = javac
MAKE  = make

FILES1    = $(shell find *.java)
TARGET1   = $(FILES1:.java=.class)
TARGETS   = $(TARGET1)

all: $(TARGETS)

%.class: %.java
	@echo "  [JAVAC] $^"
	@$(JAVAC) $(JCFLAGS1) $^

clean:
	rm -f *.class


