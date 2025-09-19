BISON=bison
FLEX=flex
CC=gcc
CFLAGS=-Wall -g

all: eiffel_lex

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c lexer.l

eiffel_lex: parser.tab.c lex.yy.c
	$(CC) $(CFLAGS) -o eiffel_lex parser.tab.c lex.yy.c -lfl

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c eiffel_lex

.PHONY: all clean
