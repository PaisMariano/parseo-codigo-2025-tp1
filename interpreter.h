#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

#define MAX_SYMBOLS 100

// Tipos de valores en tiempo de ejecución
typedef enum {
    VAL_TYPE_INT,
    VAL_TYPE_REAL,
    VAL_TYPE_STRING,
    VAL_TYPE_VOID
} ValueType;

// Estructura para un valor en tiempo de ejecución
typedef struct {
    ValueType type;
    union {
        int int_val;
        double real_val;
        char *string_val;
    } as;
} RuntimeValue;

// Entrada en la tabla de símbolos
typedef struct {
    char *name;
    RuntimeValue value;
} SymbolTableEntry; // <-- CORREGIDO: Renombrado a SymbolTableEntry

// Tabla de símbolos
typedef struct {
    SymbolTableEntry entries[MAX_SYMBOLS]; // <-- CORREGIDO: Usar SymbolTableEntry
    int count;
} SymbolTable;

// --- Prototipos de funciones del intérprete ---

// Funciones de la tabla de símbolos
void init_symbol_table(SymbolTable *table);
void set_symbol(SymbolTable *table, const char *name, RuntimeValue value);
RuntimeValue get_symbol(SymbolTable *table, const char *name);

// Función para imprimir valores
void print_value(RuntimeValue value);

// Función principal de evaluación del AST
RuntimeValue eval_ast(AstNode *node, SymbolTable *table);

#endif // INTERPRETER_H
