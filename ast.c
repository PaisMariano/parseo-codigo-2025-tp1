// Reemplaza el contenido de ast.c con esto:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

// --- Implementaciones de funciones para crear nodos ---

AstNode* create_int_literal_node(int value) {
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_INT;
    node->value.int_val = value;
    return (AstNode*)node;
}

AstNode* create_real_literal_node(double value) {
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_REAL;
    node->value.real_val = value;
    return (AstNode*)node;
}

AstNode* create_string_literal_node(char* value) {
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_STRING;
    node->value.string_val = value;
    return (AstNode*)node;
}

AstNode* create_binary_expr_node(char op, AstNode* left, AstNode* right) {
    BinaryExprNode* node = malloc(sizeof(BinaryExprNode));
    node->base.type = NODE_TYPE_BINARY_EXPR;
    node->op = op;
    node->left = left;
    node->right = right;
    return (AstNode*)node;
}

AstNode* create_procedure_call_node(char* name, ArgumentListNode* args) {
    ProcedureCallNode* node = malloc(sizeof(ProcedureCallNode));
    node->base.type = NODE_TYPE_PROCEDURE_CALL;
    node->name = name;
    node->arguments = args;
    return (AstNode*)node;
}

ArgumentListNode* create_argument_list_node(AstNode* arg, ArgumentListNode* next) {
    ArgumentListNode* node = malloc(sizeof(ArgumentListNode));
    node->base.type = NODE_TYPE_ARGUMENT_LIST;
    node->argument = arg;
    node->next = next;
    return node;
}

StatementListNode* append_to_statement_list(StatementListNode* list, AstNode* stmt) {
    StatementListNode* new_node = create_statement_list_node(stmt, NULL);
    if (list == NULL) {
        return new_node;
    }
    StatementListNode* current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return list;
}
StatementListNode* create_statement_list_node(AstNode* stmt, StatementListNode* next) {
    StatementListNode* node = malloc(sizeof(StatementListNode));
    node->base.type = NODE_TYPE_STATEMENT_LIST;
    node->statement = stmt;
    node->next = next;
    return node;
}

AstNode* create_assign_node(char* name, AstNode* expr) {
    AssignNode* node = malloc(sizeof(AssignNode));
    node->base.type = NODE_TYPE_ASSIGN;
    node->name = name;
    node->expression = expr;
    return (AstNode*)node;
}

AstNode* create_variable_node(char* name) {
    VariableNode* node = malloc(sizeof(VariableNode));
    node->base.type = NODE_TYPE_VARIABLE;
    node->name = name;
    return (AstNode*)node;
}

// --- Implementaciones para imprimir y liberar el AST ---

void print_ast(AstNode *node, FILE *output) {
    // Implementación futura
}

void free_ast(AstNode *node) {
    // Implementación futura
}
