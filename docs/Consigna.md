## Trabajo Práctico - Intérprete y Compilador

**Asignatura:** Parseo de código
**Fecha entrega:** 26 de Noviembre de 2025

# Objetivo

A partir del analizador léxico entregado en la Parte 1, implementar **un compilador
(opcional) y un intérprete** para un **subconjunto de Eiffel**. El sistema debe permitir
ejecutar los programas de prueba y producir la salida esperada.
Se pueden elegir una de estas dos opciones de entrega:
● **Opción A (Intérprete directo):** Flex + Bison → AST en C/C++ → intérprete que
ejecuta el AST (recorrido y evaluación).
● **Opción B (Compilador + VM):** Flex + Bison → AST → compilador que genera
**bytecode** para una VM simple (stack-based) + implementa la VM en C/C++. Los
tests se ejecutan con VM.
Requisitos mínimos (obligatorios):
● Soportar los elementos del subconjunto especificado.
● Implementar manejo de **entorno de variables** y **ámbito léxico** para variables
locales y atributos simples.
● Implementar **errores léxicos/sintácticos** con mensajes legibles y localización
(línea/columna).
● Proveer un Makefile con targets: all, build, run-tests, clean.
● Entregar: código fuente, Makefile, 10 archivos .e de prueba, archivo salidas.txt
y un **informe** en detalle (informe.pdf) que explique diseño y decisiones.
Opcionales (extra):
● Manejo de llamadas recursivas (para funciones con Result o return-like).
● Manejo de tipos (INTEGER/REAL/STRING) con coerciones mínimas.
● Soporte de arrays o estructuras simples.
● Optimización simple (const folding) en compilador a bytecode.

# 2) Arquitectura sugerida

Diagrama (conceptual):
Eiffel source (.e)
│
Flex (lexer)
│ tokens
Bison (parser)
│ AST (estructuras C/C++)
Front-end: semantic checks (symbol tables)
│ IR (AST o bytecode)
Back-end:

- Opción A: AST interpreter (walk & eval)
- Opción B: Bytecode generator → .bc
  VM (stack-based) executes .bc
  Componentes mínimos a implementar:
  ● **lexer.l** (Flex): Ya hecho en Parte1, ampliar si agregan tokens.
  ● **parser.y** (Bison): Gramática del subconjunto; construir AST nodes (estructuras
  struct en C).
  ● **ast.h / ast.c** : definiciones de nodos y utilidades (nuevo nodo por: Program, Class,
  Feature, Assign, If, While, Print, ExprBinary, ExprLiteral, VarRef, Call).
  ● **symtab.c** : tabla de símbolos (scopes: global/class/local).
  ● **interpreter.c** (Opción A) o **compiler.c + vm.c** (Opción B).

## ● runtime.c : funciones de runtime como print, read, manejo de strings, errores.

# 3) Subconjunto de Eiffel a implementar

# (especificación)

**Forma general de programa (aceptado):**
● Un **archivo** contiene **una clase** con al menos una feature make (o main) que será el
punto de entrada. Ejemplo:
class MAIN
feature
make
do
-- cuerpo
end
end
**Elementos del lenguaje (obligatorios):**
● Palabras clave: class, feature, do, end, create (opcional), if, then, else,
from, until, loop, print (primitiva).
● Identificadores: [A-Za-z_][A-Za-z0-9_]* (caso insensible opcional).
● Literales: enteros ( 123 ), reales (1.23), cadenas entre comillas ("hola").
● Operadores: +, - , *, /, := (asignación), comparadores =, <, >, <=, >=.
● Expresiones aritméticas y booleanas con precedencia (* antes de + etc).
● Sentencias: asignación, if ... then ... else ... end, bucle from i := 0
until i = N loop ... end (sintaxis Eiffel clásica), print(expr).
● Declaraciones de variables locales dentro de feature no requeridas en primera
versión: se puede usar variables implícitas (optativo) o exigir declaración previa
(mejor: exigir local section para variables). Para simplificar, permitimos declarar
variables en una sección local

● Llamadas a otras features del mismo objeto simple (sin parámetros obligatorios en
base versión; si se soportan parámetros se valora como extra).
**Limitaciones/Not supported initially:**
● Herencia (puede añadirse como extra).
● Tipos complejos, genéricos, excepciones avanzadas.
**Semántica esperada:**
● Tipos básicos: INTEGER, REAL, STRING. Operaciones aritméticas coherentes
(INTEGER/REAL).
● print(expr) envía la representación textual del valor a stdout seguido de \n.
● if y loop se comportan como en Eiffel clásico (evaluar condicional, ejecutar rama
correspondiente).
● En Eiffel, la estructura de control from ... until ... loop ... end puede
incluir partes opcionales como invariant y variant, destinadas a la verificación
de corrección y terminación del bucle.
Para efectos de este trabajo práctico, se adopta una versión simplificada de su
semántica operacional, omitiendo dichas secciones.La ejecución del bucle debe
interpretarse de la siguiente manera:
○ Primero se ejecuta la sección de inicialización (from). Luego, mientras
la condición especificada en until sea **falsa** , se ejecuta el cuerpo del
bucle (loop).
Cuando la condición se evalúa como **verdadera** , el bucle finaliza.
○ No se incluye una sección de actualización explícita: cualquier actualización
de variables debe realizarse dentro del cuerpo del bucle.
○ En pseudocódigo, su comportamiento puede representarse como:

# 4) Interfaz de usuario / comandos

Proporcionar un Makefile con los siguientes comandos:
● make all — compila el proyecto (genera lexer.c/parser.c, compila y linkea).
● make build — idem.
● make run FILE=salidas/hello.e — ejecuta ./interpreter salidas/hello.e (Opción A) o
./compiler salidas/hello.e && ./vm out.bc (Opción B).
● make tests — ejecuta todos los 10 tests y compara con salidas_esperadas.txt.
● make clean — limpia archivos generados.
**Ejecución mínima (sugerida):
● Opción A (intérprete directo):**
○ ./interpreter examples/hello.e
**● Opción B (bytecode):**
○ ./compiler examples/hello.e -o examples/hello.bc
○ ./vm examples/hello.bc
Documentar en el informe los pasos exactos de compilación ( flex, bison, gcc).

# 5) Los 10 archivos de ejemplo (código

# Eiffel) — NOMBRES + CÓDIGO +

# SALIDAS ESPERADAS

Cada archivo debe guardarse en salidas/ y nombrarse 01_hello.e, 02_arit.e, etc.
A continuación se muestran los ejemplos y las salidas esperadas en cada ejecución.

## 01_hello.e — Hello world básico

class MAIN
feature
make
do
print("Hola, mundo Eiffel!")
end

end
**Salida esperada:**
Hola, mundo Eiffel!

## 02_arit.e — Aritmética simple

class MAIN
feature
make
do
print( 1 + 2 * 3 ) -- debe respetar precedencia
end
end
**Salida esperada:**
7

## 03_vars.e — Variables y asignaciones

class MAIN
feature
make
local
x, y: INTEGER
do
x := 10
y := x + 5
print(y)
end
end
**Salida esperada:**
15

## 04_if_else.e — If / else

class MAIN
feature
make
local
x: INTEGER
do
x := 4
if x > 0 then
print("positivo")
else
print("no positivo")
end
end
end
**Salida esperada:**
positivo

## 05_loop_until.e — Bucle from/until/loop

class MAIN
feature
make
local
i: INTEGER
do
i := 0
from
i := 0
until
i = 3
loop
print(i)
i := i + 1
end
end
end
**Salida esperada:** (una línea por print)

### 0

### 1

### 2

## 06_fib_iter.e — Fibonacci iterativo

class MAIN
feature
make
local
a, b, t, i, n: INTEGER
do
n := 6
a := 0
b := 1
from
i := 0
until
i = n
loop
print(a)
t := a + b
a := b
b := t
i := i + 1
end
end
end
**Salida esperada:**
0 1 1 2 3 5

## 07_fact_rec.e — Factorial (recursivo, extra si se

## implementan funciones)

**Obs** : este ejemplo requiere que implementen llamadas a funciones/feature con
parámetros y Result o retorno. Si no desean implementar la recursión, indicar
como **extra**.
class FACT
feature
factorial (n: INTEGER): INTEGER
do
if n = 0 then
Result := 1
else
Result := n * factorial(n - 1)
end
end
end
class MAIN
feature
make
local
f: FACT
do
create f
print(f.factorial(5))
end
end
**Salida esperada:**
120
(Si no se implementan funciones, este test será opcional/extra.)

## 08_strings.e — Manejo de strings

class MAIN
feature
make

do
print("Hola " + "mundo")
print("número: " + " " + "42")
end
end
**Salida esperada:**
Hola mundo
número: 42
(Obs: concatenación simple definida como + entre strings; si no implementan sobrecarga,
pueden usar print("Hola mundo") como alternativa.)

## 09_class_attr.e — Clase con atributo y feature (atributo

## simple)

class COUNTER
feature
value: INTEGER
inc
do
value := value + 1
end
end
class MAIN
feature
make
local
c: COUNTER
do
create c
c.value := 5
c.inc
print(c.value)
end
end

**Salida esperada:**
6
**Implementación mínima (opcional):** Se requiere manejar objetos con almacenamiento de
atributos. En caso de que la implementación completa de objetos resulte demasiado
compleja, puede simplificarse utilizando variables globales equivalentes (por ejemplo,
c_value). Esta decisión debe documentarse en el informe.

## 10_errors.e — Test de errores (sintáctico / semántico)

class MAIN
feature
make
do
x := 5 -- error: x no declarado
end
end
**Salida esperada (mensaje de error legible):**
Runtime/Semantic error: variable 'x' not declared (line 3)

## Entregables

1. Código fuente completo (Flex + Bison + C/C++ / VM).
2. Makefile funcional.
3. 10 archivos en examples/ con los nombres indicados.
4. expected_outputs.txt.
5. Un informe en pdf bien en detalle que contenga las soluciones implementadas así
   como también arquitectura, decisiones (interpretación vs bytecode), descripción de
   nodos AST, manejo de ámbitos, problemas encontrados, y capturas de ejecución de
   los ejemplos.
   **(Opcional) tests adicionales y extras documentados.**

## Criterios de evaluación

```
● Corrección formal de los desarrollos.
● Claridad en la explicación de pasos y diagramas.
● Presentación ordenada, completa y coherente del informe.
● Profundidad en la solución.
```

