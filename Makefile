BISON=bison
FLEX=flex
CC=gcc
CFLAGS=-Wall -g
UNAME_S := $(shell uname -s)

# El ejecutable principal ahora es 'interpreter'
TARGET=interpreter

# Archivos fuente
SOURCES=parser.tab.c lex.yy.c ast.c interpreter.c main.c

all: $(TARGET)

# Regla para generar el parser y el lexer
parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c lexer.l

# Regla para compilar y enlazar el intérprete completo
$(TARGET): parser.tab.c lex.yy.c ast.c interpreter.c main.c
ifeq ($(UNAME_S),Darwin)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
else
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -lfl
endif

# Nuevo target para ejecutar una prueba específica del intérprete
test: $(TARGET)
		@echo "--- Running Interpreter Tests in tests/TP2 ---"
		@for t in tests/TP2/*.e; do \
			echo "Running test $$t..."; \
			./$(TARGET) $$t > $$t.result; \
			if diff -q $$t.result $$t.expected > /dev/null; then \
				echo "  ✅ PASSED"; \
				rm -f $$t.result; \
			else \
				echo "  ❌ FAILED (see $$t.result vs $$t.expected)"; \
			fi \
		done

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c $(TARGET) tests/TP2/*.result tests/TP2/*.info

.PHONY: all clean test test-interpreter
