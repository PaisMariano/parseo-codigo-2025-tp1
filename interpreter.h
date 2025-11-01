#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

// Estructura para representar un valor en tiempo de ejecución
typedef enum { VAL_TYPE_INT, VAL_TYPE_REAL, VAL_TYPE_STRING, VAL_TYPE_VOID } ValueType;

typedef struct {
    ValueType type;
    union {
        int int_val;
        double real_val;
        char *string_val;
    } as;
} RuntimeValue;

// Función principal del intérprete: evalúa un nodo del AST
RuntimeValue eval_ast(AstNode *node);

// Función para imprimir un valor
void print_value(RuntimeValue value);

#endif // INTERPRETER_H
