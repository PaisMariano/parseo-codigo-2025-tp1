BISON=bison
FLEX=flex
CC=gcc
CFLAGS=-Wall -g
UNAME_S := $(shell uname -s)

all: eiffel_lex

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c lexer.l

eiffel_lex: parser.tab.c lex.yy.c
ifeq ($(UNAME_S),Darwin)
	$(CC) $(CFLAGS) -o eiffel_lex parser.tab.c lex.yy.c
else
	$(CC) $(CFLAGS) -o eiffel_lex parser.tab.c lex.yy.c -lfl
endif

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c eiffel_lex

test: eiffel_lex
	@echo "Ejecutando tests..."
	@for f in tests/*.e; do \
		echo "==== $$f ===="; \
		./eiffel_lex < $$f; \
	done

.PHONY: all clean test
