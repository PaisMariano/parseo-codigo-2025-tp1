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

AstNode* create_if_node(AstNode* condition, StatementListNode* then_branch, StatementListNode* else_branch) {
    IfNode* node = malloc(sizeof(IfNode));
    node->base.type = NODE_TYPE_IF;
    node->condition = condition;
    node->then_branch = then_branch;
    node->else_branch = else_branch;
    return (AstNode*)node;
}

AstNode* create_comparison_expr_node(int op, AstNode* left, AstNode* right) {
    ComparisonExprNode* node = malloc(sizeof(ComparisonExprNode));
    node->base.type = NODE_TYPE_COMPARISON_EXPR;
    node->op = op;
    node->left = left;
    node->right = right;
    return (AstNode*)node;
}

// --- NUEVAS IMPLEMENTACIONES ---

AstNode* create_loop_node(StatementListNode* init, AstNode* condition, StatementListNode* body) {
    LoopNode* node = malloc(sizeof(LoopNode));
    node->base.type = NODE_TYPE_LOOP;
    node->initialization = init;
    node->condition = condition;
    node->loop_body = body;
    return (AstNode*)node;
}

AstNode* create_attribute_access_node(char* obj_name, char* attr_name) {
    AttributeAccessNode* node = malloc(sizeof(AttributeAccessNode));
    node->base.type = NODE_TYPE_ATTRIBUTE_ACCESS;
    node->object_name = obj_name;
    node->attribute_name = attr_name;
    return (AstNode*)node;
}

AstNode* create_method_call_node(char* obj_name, char* method_name, ArgumentListNode* args) {
    MethodCallNode* node = malloc(sizeof(MethodCallNode));
    node->base.type = NODE_TYPE_METHOD_CALL;
    node->object_name = obj_name;
    node->method_name = method_name;
    node->arguments = args;
    return (AstNode*)node;
}

AstNode* create_create_node(char* obj_name) {
    CreateNode* node = malloc(sizeof(CreateNode));
    node->base.type = NODE_TYPE_CREATE;
    node->object_name = obj_name;
    return (AstNode*)node;
}

ArgumentListNode* reverse_argument_list(ArgumentListNode* list) {
    ArgumentListNode *prev = NULL, *current = list, *next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}


// --- Implementaciones para imprimir y liberar el AST ---

void print_ast(AstNode *node, FILE *output) {
    // Implementación futura
}

void free_ast(AstNode *node) {
    // Implementación futura
}
