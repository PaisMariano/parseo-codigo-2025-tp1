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
    if (list == NULL) {
        return create_statement_list_node(stmt, NULL);
    }
    StatementListNode* current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = create_statement_list_node(stmt, NULL);
    return list;
}
StatementListNode* create_statement_list_node(AstNode* stmt, StatementListNode* next) {
    StatementListNode* node = malloc(sizeof(StatementListNode));
    node->base.type = NODE_TYPE_STATEMENT_LIST;
    node->statement = stmt;
    node->next = next;
    return node;
}

AstNode* create_assign_node(AstNode* target, AstNode* expr) {
    AssignNode* node = malloc(sizeof(AssignNode));
    node->base.type = NODE_TYPE_ASSIGN;
    node->target = target;
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

AstNode* create_loop_node(StatementListNode* init, AstNode* condition, StatementListNode* body) {
    LoopNode* node = malloc(sizeof(LoopNode));
    node->base.type = NODE_TYPE_LOOP;
    node->initialization = init;
    node->condition = condition;
    node->loop_body = body;
    return (AstNode*)node;
}

AstNode* create_attribute_access_node(AstNode* obj_node, char* attr_name) {
    AttributeAccessNode* node = malloc(sizeof(AttributeAccessNode));
    node->base.type = NODE_TYPE_ATTRIBUTE_ACCESS;
    node->object_node = obj_node;
    node->attribute_name = attr_name;
    return (AstNode*)node;
}

AstNode* create_method_call_node(AstNode* obj_node, char* method_name, ArgumentListNode* args) {
    MethodCallNode* node = malloc(sizeof(MethodCallNode));
    node->base.type = NODE_TYPE_METHOD_CALL;
    node->object_node = obj_node;
    node->method_name = method_name;
    node->arguments = args;
    return (AstNode*)node;
}

AstNode* create_create_node(char* obj_name) {
    CreateNode* node = malloc(sizeof(CreateNode));
    node->base.type = NODE_TYPE_CREATE;
    node->object_name = obj_name;
    node->class_name = NULL; // Se resolverá en análisis semántico (o en el intérprete por ahora)
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

// --- IMPLEMENTACIONES PARA DECLARACIONES ---

DeclarationListNode* create_declaration_list_node(char* name, DeclarationListNode* next) {
    DeclarationListNode* node = malloc(sizeof(DeclarationListNode));
    node->base.type = NODE_TYPE_DECLARATION_LIST;
    node->variable_name = name;
    node->type_name = NULL; // NUEVO: por defecto sin tipo
    node->next = next;
    return node;
}

DeclarationListNode* append_to_declaration_list(DeclarationListNode* list, DeclarationListNode* new_decls) {
    if (!list) return new_decls;
    DeclarationListNode* current = list;
    while (current->next) {
        current = current->next;
    }
    current->next = new_decls;
    return list;
}

// NUEVA FUNCIÓN: asigna type_name a todos los nodos de la lista
void set_declaration_type(DeclarationListNode* list, char* type_name) {
    DeclarationListNode* cur = list;
    while (cur) {
        if (type_name) cur->type_name = strdup(type_name);
        cur = cur->next;
    }
}

AstNode* create_feature_body_node(DeclarationListNode* decls, StatementListNode* stmts) {
    FeatureBodyNode* node = malloc(sizeof(FeatureBodyNode));
    node->base.type = NODE_TYPE_FEATURE_BODY;
    node->declarations = decls;
    node->statements = stmts;
    node->feature_name = NULL;
    return (AstNode*)node;
}

AstNode* create_class_node(char* name, StatementListNode* features) {
    ClassNode* node = malloc(sizeof(ClassNode));
    node->base.type = NODE_TYPE_CLASS_DECL;
    node->name = name;
    node->features = features;
    return (AstNode*)node;
}


// --- Implementaciones para imprimir y liberar el AST ---

#include <stdarg.h>

static void print_indent(FILE *output, int indent) {
    for (int i = 0; i < indent; ++i) fputc(' ', output);
}

static void free_ast_internal(AstNode *node);

static void free_statement_list(StatementListNode* list) {
    StatementListNode *s = list;
    while (s) {
        StatementListNode *next = s->next;
        free_ast_internal(s->statement);
        free(s);
        s = next;
    }
}

static void free_ast_internal(AstNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_TYPE_LITERAL: {
            LiteralNode *n = (LiteralNode*)node;
            if (n->literal_type == LITERAL_TYPE_STRING && n->value.string_val) {
                free(n->value.string_val);
            }
            break;
        }
        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *n = (BinaryExprNode*)node;
            free_ast_internal(n->left);
            free_ast_internal(n->right);
            break;
        }
        case NODE_TYPE_COMPARISON_EXPR: {
            ComparisonExprNode *n = (ComparisonExprNode*)node;
            free_ast_internal(n->left);
            free_ast_internal(n->right);
            break;
        }
        case NODE_TYPE_PROCEDURE_CALL: {
            ProcedureCallNode *n = (ProcedureCallNode*)node;
            if (n->name) free(n->name);
            ArgumentListNode *arg = n->arguments;
            while (arg) {
                ArgumentListNode *next = arg->next;
                free_ast_internal(arg->argument);
                free(arg);
                arg = next;
            }
            break;
        }
        case NODE_TYPE_METHOD_CALL: {
            MethodCallNode *n = (MethodCallNode*)node;
            free_ast_internal(n->object_node);
            if (n->method_name) free(n->method_name);
            ArgumentListNode *arg = n->arguments;
            while (arg) {
                ArgumentListNode *next = arg->next;
                free_ast_internal(arg->argument);
                free(arg);
                arg = next;
            }
            break;
        }
        case NODE_TYPE_ATTRIBUTE_ACCESS: {
            AttributeAccessNode *n = (AttributeAccessNode*)node;
            free_ast_internal(n->object_node);
            if (n->attribute_name) free(n->attribute_name);
            break;
        }
        case NODE_TYPE_CREATE: {
            CreateNode *n = (CreateNode*)node;
            if (n->object_name) free(n->object_name);
            if (n->class_name) free(n->class_name);
            break;
        }
        case NODE_TYPE_VARIABLE: {
            VariableNode *n = (VariableNode*)node;
            if (n->name) free(n->name);
            break;
        }
        case NODE_TYPE_ASSIGN: {
            AssignNode *n = (AssignNode*)node;
            free_ast_internal(n->target);
            free_ast_internal(n->expression);
            break;
        }
        case NODE_TYPE_IF: {
            IfNode *n = (IfNode*)node;
            free_ast_internal(n->condition);
            free_statement_list(n->then_branch);
            free_statement_list(n->else_branch);
            break;
        }
        case NODE_TYPE_LOOP: {
            LoopNode *n = (LoopNode*)node;
            free_statement_list(n->initialization);
            free_ast_internal(n->condition);
            free_statement_list(n->loop_body);
            break;
        }
        case NODE_TYPE_STATEMENT_LIST: {
            free_statement_list((StatementListNode*)node);
            return; // El bucle ya libera el nodo
        }
        case NODE_TYPE_DECLARATION_LIST: {
            DeclarationListNode *it = (DeclarationListNode*)node;
            while (it) {
                DeclarationListNode *next = it->next;
                if (it->variable_name) free(it->variable_name);
                if (it->type_name) free(it->type_name); // LIBERAR type_name
                free(it);
                it = next;
            }
            return; // El bucle ya libera el nodo
        }
        case NODE_TYPE_FEATURE_BODY: {
            FeatureBodyNode *n = (FeatureBodyNode*)node;
            if (n->feature_name) free(n->feature_name);
            if (n->declarations) free_ast_internal((AstNode*)n->declarations);
            if (n->statements) free_ast_internal((AstNode*)n->statements);
            break;
        }
        case NODE_TYPE_CLASS_DECL: {
            ClassNode *n = (ClassNode*)node;
            if (n->name) free(n->name);
            if (n->features) free_ast_internal((AstNode*)n->features);
            break;
        }
        default:
            break;
    }
    free(node);
}

void free_ast(AstNode *node) {
    free_ast_internal(node);
}
