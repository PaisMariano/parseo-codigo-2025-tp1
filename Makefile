BISON=bison
FLEX=flex
CC=gcc
CFLAGS=-Wall -g
UNAME_S := $(shell uname -s)

# El ejecutable principal ahora es 'interpreter'
TARGET=interpreter

# Archivos fuente
SOURCES=parser.tab.c lex.yy.c ast.c interpreter.c

# Archivos de prueba para el target 'test' original
TESTS := $(basename $(wildcard tests/*.e))

all: $(TARGET)

# Regla para generar el parser y el lexer
parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c lexer.l

# Regla para compilar y enlazar el intérprete completo
$(TARGET): parser.tab.c lex.yy.c ast.c interpreter.c
ifeq ($(UNAME_S),Darwin)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
else
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) -lfl
endif

# Nuevo target para ejecutar una prueba específica del intérprete
test-interpreter: $(TARGET)
	@echo "--- Running Interpreter Test ---"
	./$(TARGET) tests/test-interpreter.e

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c $(TARGET)

# El target 'test' original se mantiene por si es necesario
test: $(TARGET)
	@for t in $(TESTS); do \
		echo "Running test $$t..."; \
		./$(TARGET) < $$t.e > $$t.result; \
		if diff -q $$t.result $$t.expected > /dev/null; then \
			echo "  ✅ PASSED"; \
			rm -f $$t.result; \
		else \
			echo "  ❌ FAILED (see $$t.result vs $$t.expected)"; \
		fi \
	done

.PHONY: all clean test test-interpreter
