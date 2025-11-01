#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

// Función para imprimir un valor en consola
void print_value(RuntimeValue value) {
    switch (value.type) {
        case VAL_TYPE_INT:
            printf("%d\n", value.as.int_val);
            break;
        case VAL_TYPE_REAL:
            printf("%f\n", value.as.real_val);
            break;
        case VAL_TYPE_STRING:
            printf("%s\n", value.as.string_val);
            break;
        case VAL_TYPE_VOID:
            // No imprimir nada para void
            break;
    }
}

// Función principal que recorre y evalúa el AST
RuntimeValue eval_ast(AstNode *node) {
    RuntimeValue result = { .type = VAL_TYPE_VOID }; // Valor por defecto

    if (!node) {
        return result;
    }

    switch (node->type) {
        case NODE_TYPE_LITERAL: {
            LiteralNode *lit_node = (LiteralNode*) node;
            if (lit_node->literal_type == LITERAL_TYPE_INT) {
                result.type = VAL_TYPE_INT;
                result.as.int_val = lit_node->value.int_val;
            }
            // Aquí agregarías casos para REAL y STRING
            break;
        }

        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *bin_expr = (BinaryExprNode*) node;
            RuntimeValue left_val = eval_ast(bin_expr->left);
            RuntimeValue right_val = eval_ast(bin_expr->right);

            // Por ahora, solo operamos con enteros
            if (left_val.type == VAL_TYPE_INT && right_val.type == VAL_TYPE_INT) {
                result.type = VAL_TYPE_INT;
                switch (bin_expr->op) {
                    case '+': result.as.int_val = left_val.as.int_val + right_val.as.int_val; break;
                    case '-': result.as.int_val = left_val.as.int_val - right_val.as.int_val; break;
                    case '*': result.as.int_val = left_val.as.int_val * right_val.as.int_val; break;
                    case '/': result.as.int_val = left_val.as.int_val / right_val.as.int_val; break;
                }
            }
            break;
        }

        // Aquí agregarías los 'case' para IF, LOOP, ASSIGN, etc.
    }

    return result;
}
