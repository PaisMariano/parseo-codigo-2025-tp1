#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// Crear nodo de expresión binaria
AstNode* create_binary_expr_node(char op, AstNode* left, AstNode* right) {
    BinaryExprNode* node = malloc(sizeof(BinaryExprNode));
    node->base.type = NODE_TYPE_BINARY_EXPR;
    node->op = op;
    node->left = left;
    node->right = right;
    return (AstNode*)node;
}

// Crear nodo de literal entero
AstNode* create_int_literal_node(int value) {
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_INT;
    node->value.int_val = value;
    return (AstNode*)node;
}

// Crear nodo de literal real (nuevo)
AstNode* create_real_literal_node(double value) {
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_REAL;
    node->value.real_val = value;
    return (AstNode*)node;
}

// Crear nodo de literal string (nuevo)
AstNode* create_string_literal_node(char* value) {
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_STRING;
    node->value.string_val = value; // strdup si el original se libera
    return (AstNode*)node;
}

// Liberar memoria del AST (recursivamente)
void free_ast(AstNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode* bin_expr = (BinaryExprNode*)node;
            free_ast(bin_expr->left);
            free_ast(bin_expr->right);
            break;
        }
        case NODE_TYPE_LITERAL: {
            LiteralNode* lit_node = (LiteralNode*)node;
            if (lit_node->literal_type == LITERAL_TYPE_STRING) {
                free(lit_node->value.string_val);
            }
            break;
        }
    }
    free(node);
}
