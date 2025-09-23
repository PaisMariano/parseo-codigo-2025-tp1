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

TESTS := $(basename $(wildcard tests/*.e))

test: eiffel_lex
	@for t in $(TESTS); do \
		echo "Running test $$t..."; \
		./eiffel_lex < $$t.e > $$t.result; \
		if diff -q $$t.result $$t.expected > /dev/null; then \
			echo "  ✅ PASSED"; \
			rm -f $$t.result; \
		else \
			echo "  ❌ FAILED (see $$t.result vs $$t.expected)"; \
		fi \
	done

.PHONY: all clean test
