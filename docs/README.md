# Trabajo Práctico – Analizador Léxico Eiffel

Este proyecto implementa un **analizador léxico** para el lenguaje Eiffel, utilizando **Flex** y **Bison**.  
En esta primera parte solo se requiere el *lexer* (Flex).

---

## 📂 Estructura del proyecto

parseo-codigo-2025-tp1/
├── docs/
│   └── eiffel_tokens.md      # Documentación de tokens definidos
│   └── Informe Trabajo Practico  # Informe del trabajo práctico
├── tests/
│   ├── hello.e               # Programa Eiffel de prueba
│   ├── hello.expected          # Salida esperada del lexer
├── lexer.l                   # Definiciones léxicas (Flex)
├── parser.y                  # Gramática (Bison, mínima en esta etapa)
├── parser.tab.c              # Código generado por Bison
├── parser.tab.h              # Tokens generados por Bison
├── lex.yy.c                  # Código generado por Flex
├── eiffel_lex                # Ejecutable (lexer compilado)
├── Makefile                  # Reglas de compilación y test

---

### ⚙️ Dependencias

- [Flex](https://github.com/westes/flex)
- [Bison](https://www.gnu.org/software/bison/)
- [GCC](https://gcc.gnu.org/)

En macOS con **Homebrew**:

```bash
brew install flex bison
```
En Ubuntu/Debian:

```bash
sudo apt-get install flex bison build-essential
```
### 🚀 Compilación

Generar los archivos a partir de Flex y Bison:

```bash flex lexer.l          # genera lex.yy.c
bison -d parser.y     # genera parser.tab.c y parser.tab.h
```

Compilar todo:

```
gcc lex.yy.c parser.tab.c -o eiffel_lex -lfl
```

También podés usar:

``` make ```


### ▶️ Ejecución del lexer

Para correr el lexer sobre un archivo Eiffel:
```
./eiffel_lex < tests/hello.e
```

Esto imprime en consola los tokens reconocidos, por ejemplo:

```
TOKEN_CLASS -> class
TOKEN_IDENTIFIER -> HELLO_WORLD
TOKEN_CREATE -> create
...
```

### ✅ Tests

Cada archivo .e en tests/ tiene un archivo .output con la salida esperada.

Podés verificar corriendo:


``` 
./eiffel_lex < tests/hello.e > tests/hello.result
diff tests/hello.result tests/hello.output 
```

Si no hay diferencias, el lexer funciona correctamente.

Con Makefile:

```
make test
```
### 📌 Notas técnicas
	•	El lexer reconoce:
	•	Keywords: class, create, feature, do, end, if, then, else, from, until, loop, inherit, require, ensure, local, alias, is, when, check, not, and, or…
	•	Identificadores: [A-Za-z_][A-Za-z0-9_]*
	•	Números: enteros, reales y notación científica
	•	Strings con secuencias de escape %N, %T, %\", %%
	•	Operadores: :=, =, <, >, <=, >=, +, -, *, /
	•	Símbolos: (, ), ;, ,, :, .
	•	Comentarios: -- hasta fin de línea
	•	Espacios en blanco y tabs son ignorados.
	•	Caracteres no reconocidos se reportan como TOKEN_UNKNOWN.

