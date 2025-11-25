#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Comentario general:
   - Este archivo implementa "constructores" de nodos del AST, utilidades (append/reverse)
     y funciones para imprimir y liberar el AST.
   - Las funciones create_* devuelven punteros a nodos ya inicializados.
*/

/* --- Implementaciones de funciones para crear nodos --- */

AstNode* create_int_literal_node(int value) {
    /* Crea un nodo literal entero */
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_INT;
    node->value.int_val = value;
    return (AstNode*)node;
}

AstNode* create_real_literal_node(double value) {
    /* Crea un nodo literal real */
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_REAL;
    node->value.real_val = value;
    return (AstNode*)node;
}

AstNode* create_string_literal_node(char* value) {
    /* Crea un nodo literal de cadena */
    LiteralNode* node = malloc(sizeof(LiteralNode));
    node->base.type = NODE_TYPE_LITERAL;
    node->literal_type = LITERAL_TYPE_STRING;
    node->value.string_val = value;
    return (AstNode*)node;
}

AstNode* create_binary_expr_node(char op, AstNode* left, AstNode* right) {
    /* Crea un nodo de expresión binaria */
    BinaryExprNode* node = malloc(sizeof(BinaryExprNode));
    node->base.type = NODE_TYPE_BINARY_EXPR;
    node->op = op;
    node->left = left;
    node->right = right;
    return (AstNode*)node;
}

AstNode* create_procedure_call_node(char* name, ArgumentListNode* args) {
    /* Crea un nodo de llamada a procedimiento */
    ProcedureCallNode* node = malloc(sizeof(ProcedureCallNode));
    node->base.type = NODE_TYPE_PROCEDURE_CALL;
    node->name = name;
    node->arguments = args;
    return (AstNode*)node;
}

ArgumentListNode* create_argument_list_node(AstNode* arg, ArgumentListNode* next) {
    /* Crea un nodo de lista de argumentos */
    ArgumentListNode* node = malloc(sizeof(ArgumentListNode));
    node->base.type = NODE_TYPE_ARGUMENT_LIST;
    node->argument = arg;
    node->next = next;
    return node;
}

StatementListNode* append_to_statement_list(StatementListNode* list, AstNode* stmt) {
    /* Añade una sentencia al final de la lista de sentencias */
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
    /* Crea un nodo de lista de sentencias */
    StatementListNode* node = malloc(sizeof(StatementListNode));
    node->base.type = NODE_TYPE_STATEMENT_LIST;
    node->statement = stmt;
    node->next = next;
    return node;
}

AstNode* create_assign_node(AstNode* target, AstNode* expr) {
    /* Crea un nodo de asignación */
    AssignNode* node = malloc(sizeof(AssignNode));
    node->base.type = NODE_TYPE_ASSIGN;
    node->target = target;
    node->expression = expr;
    return (AstNode*)node;
}

AstNode* create_variable_node(char* name) {
    /* Crea un nodo de variable */
    VariableNode* node = malloc(sizeof(VariableNode));
    node->base.type = NODE_TYPE_VARIABLE;
    node->name = name;
    return (AstNode*)node;
}

AstNode* create_if_node(AstNode* condition, StatementListNode* then_branch, StatementListNode* else_branch) {
    /* Crea un nodo de estructura if-then-else */
    IfNode* node = malloc(sizeof(IfNode));
    node->base.type = NODE_TYPE_IF;
    node->condition = condition;
    node->then_branch = then_branch;
    node->else_branch = else_branch;
    return (AstNode*)node;
}

AstNode* create_comparison_expr_node(int op, AstNode* left, AstNode* right) {
    /* Crea un nodo de expresión de comparación */
    ComparisonExprNode* node = malloc(sizeof(ComparisonExprNode));
    node->base.type = NODE_TYPE_COMPARISON_EXPR;
    node->op = op;
    node->left = left;
    node->right = right;
    return (AstNode*)node;
}

AstNode* create_loop_node(StatementListNode* init, AstNode* condition, StatementListNode* body) {
    /* Crea un nodo de bucle (loop) */
    LoopNode* node = malloc(sizeof(LoopNode));
    node->base.type = NODE_TYPE_LOOP;
    node->initialization = init;
    node->condition = condition;
    node->loop_body = body;
    return (AstNode*)node;
}

AstNode* create_attribute_access_node(AstNode* obj_node, char* attr_name) {
    /* Crea un nodo de acceso a atributo */
    AttributeAccessNode* node = malloc(sizeof(AttributeAccessNode));
    node->base.type = NODE_TYPE_ATTRIBUTE_ACCESS;
    node->object_node = obj_node;
    node->attribute_name = attr_name;
    return (AstNode*)node;
}

AstNode* create_method_call_node(AstNode* obj_node, char* method_name, ArgumentListNode* args) {
    /* Crea un nodo de llamada a método */
    MethodCallNode* node = malloc(sizeof(MethodCallNode));
    node->base.type = NODE_TYPE_METHOD_CALL;
    node->object_node = obj_node;
    node->method_name = method_name;
    node->arguments = args;
    return (AstNode*)node;
}

AstNode* create_create_node(char* obj_name) {
    /* Crea un nodo de creación de objeto */
    CreateNode* node = malloc(sizeof(CreateNode));
    node->base.type = NODE_TYPE_CREATE;
    node->object_name = obj_name;
    node->class_name = NULL; // Se resolverá en análisis semántico (o en el intérprete por ahora)
    return (AstNode*)node;
}

ArgumentListNode* reverse_argument_list(ArgumentListNode* list) {
    /* Invierte una lista encadenada de argumentos (útil porque el parser construye en orden inverso) */
    ArgumentListNode *prev = NULL, *current = list, *next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

/* --- IMPLEMENTACIONES PARA DECLARACIONES --- */

/* create_declaration_list_node: construye listas de declaraciones (variables) */
DeclarationListNode* create_declaration_list_node(char* name, DeclarationListNode* next) {
    DeclarationListNode* node = malloc(sizeof(DeclarationListNode));
    node->base.type = NODE_TYPE_DECLARATION_LIST;
    node->variable_name = name;
    node->type_name = NULL; // por defecto sin tipo
    node->next = next;
    return node;
}

DeclarationListNode* append_to_declaration_list(DeclarationListNode* list, DeclarationListNode* new_decls) {
    /* Añade nuevas declaraciones al final de la lista de declaraciones */
    if (!list) return new_decls;
    DeclarationListNode* current = list;
    while (current->next) {
        current = current->next;
    }
    current->next = new_decls;
    return list;
}

/* NUEVA FUNCIÓN: asigna type_name a todos los nodos de la lista */
void set_declaration_type(DeclarationListNode* list, char* type_name) {
    DeclarationListNode* cur = list;
    while (cur) {
        if (type_name) cur->type_name = strdup(type_name);
        cur = cur->next;
    }
}

AstNode* create_feature_body_node(DeclarationListNode* decls, StatementListNode* stmts) {
    /* Crea un nodo de cuerpo de feature (con declaraciones y sentencias) */
    FeatureBodyNode* node = malloc(sizeof(FeatureBodyNode));
    node->base.type = NODE_TYPE_FEATURE_BODY;
    node->declarations = decls;
    node->statements = stmts;
    node->feature_name = NULL;
    return (AstNode*)node;
}

AstNode* create_class_node(char* name, StatementListNode* features) {
    /* Crea un nodo de declaración de clase */
    ClassNode* node = malloc(sizeof(ClassNode));
    node->base.type = NODE_TYPE_CLASS_DECL;
    node->name = name;
    node->features = features;
    return (AstNode*)node;
}


/* --- Implementaciones para imprimir y liberar el AST ---

   print_ast / print_ast_internal:
   - Recorren el AST recursivamente e imprimen información legible.
   - Útil para depuración y para el archivo .info que genera main.c.
*/

#include <stdarg.h>

static void print_indent(FILE *output, int indent) {
    for (int i = 0; i < indent; ++i) fputc(' ', output);
}

static void print_ast_internal(AstNode *node, FILE *output, int indent);

void print_ast(AstNode *node, FILE *output) {
    if (!output) return;
    print_ast_internal(node, output, 0);
}

static void print_ast_internal(AstNode *node, FILE *output, int indent) {
    if (!node) {
        print_indent(output, indent);
        fprintf(output, "(null)\n");
        return;
    }

    print_indent(output, indent);

    switch (node->type) {
        case NODE_TYPE_LITERAL: {
            LiteralNode *n = (LiteralNode*)node;
            fprintf(output, "Literal: ");
            switch (n->literal_type) {
                case LITERAL_TYPE_INT: fprintf(output, "%d (int)\n", n->value.int_val); break;
                case LITERAL_TYPE_REAL: fprintf(output, "%f (real)\n", n->value.real_val); break;
                case LITERAL_TYPE_STRING: fprintf(output, "\"%s\" (string)\n", n->value.string_val); break;
            }
            break;
        }
        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *n = (BinaryExprNode*)node;
            fprintf(output, "BinaryExpr: %c\n", n->op);
            print_ast_internal(n->left, output, indent + 2);
            print_ast_internal(n->right, output, indent + 2);
            break;
        }
        case NODE_TYPE_COMPARISON_EXPR: {
            ComparisonExprNode *n = (ComparisonExprNode*)node;
            fprintf(output, "ComparisonExpr: %d\n", n->op);
            print_ast_internal(n->left, output, indent + 2);
            print_ast_internal(n->right, output, indent + 2);
            break;
        }
        case NODE_TYPE_PROCEDURE_CALL: {
            ProcedureCallNode *n = (ProcedureCallNode*)node;
            fprintf(output, "ProcedureCall: %s\n", n->name);
            print_ast_internal((AstNode*)n->arguments, output, indent + 2);
            break;
        }
        case NODE_TYPE_METHOD_CALL: {
            MethodCallNode *n = (MethodCallNode*)node;
            fprintf(output, "MethodCall: %s\n", n->method_name);
            print_indent(output, indent + 2);
            fprintf(output, "Object:\n");
            print_ast_internal(n->object_node, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Arguments:\n");
            print_ast_internal((AstNode*)n->arguments, output, indent + 4);
            break;
        }
        case NODE_TYPE_ARGUMENT_LIST: {
            ArgumentListNode *list = (ArgumentListNode*)node;
            fprintf(output, "ArgumentList\n");
            while (list) {
                print_ast_internal(list->argument, output, indent + 2);
                list = list->next;
            }
            break;
        }
        case NODE_TYPE_STATEMENT_LIST: {
            StatementListNode *list = (StatementListNode*)node;
            fprintf(output, "StatementList\n");
            while (list) {
                print_ast_internal(list->statement, output, indent + 2);
                list = list->next;
            }
            break;
        }
        case NODE_TYPE_ASSIGN: {
            AssignNode *n = (AssignNode*)node;
            fprintf(output, "Assign\n");
            print_indent(output, indent + 2);
            fprintf(output, "Target:\n");
            print_ast_internal(n->target, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Expression:\n");
            print_ast_internal(n->expression, output, indent + 4);
            break;
        }
        case NODE_TYPE_VARIABLE: {
            VariableNode *n = (VariableNode*)node;
            fprintf(output, "Variable: %s\n", n->name);
            break;
        }
        case NODE_TYPE_IF: {
            IfNode *n = (IfNode*)node;
            fprintf(output, "If\n");
            print_indent(output, indent + 2);
            fprintf(output, "Condition:\n");
            print_ast_internal(n->condition, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Then:\n");
            print_ast_internal((AstNode*)n->then_branch, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Else:\n");
            print_ast_internal((AstNode*)n->else_branch, output, indent + 4);
            break;
        }
        case NODE_TYPE_LOOP: {
            LoopNode *n = (LoopNode*)node;
            fprintf(output, "Loop\n");
            print_indent(output, indent + 2);
            fprintf(output, "Initialization:\n");
            print_ast_internal((AstNode*)n->initialization, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Condition:\n");
            print_ast_internal(n->condition, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Body:\n");
            print_ast_internal((AstNode*)n->loop_body, output, indent + 4);
            break;
        }
        case NODE_TYPE_ATTRIBUTE_ACCESS: {
            AttributeAccessNode *n = (AttributeAccessNode*)node;
            fprintf(output, "AttributeAccess: %s\n", n->attribute_name);
            print_ast_internal(n->object_node, output, indent + 2);
            break;
        }
        case NODE_TYPE_CREATE: {
            CreateNode *n = (CreateNode*)node;
            fprintf(output, "Create: %s\n", n->object_name);
            break;
        }
        case NODE_TYPE_DECLARATION_LIST: {
            DeclarationListNode *list = (DeclarationListNode*)node;
            fprintf(output, "DeclarationList\n");
            while(list) {
                print_indent(output, indent + 2);
                fprintf(output, "Var: %s, Type: %s\n", list->variable_name, list->type_name ? list->type_name : "(none)");
                list = list->next;
            }
            break;
        }
        case NODE_TYPE_FEATURE_BODY: {
            FeatureBodyNode *n = (FeatureBodyNode*)node;
            fprintf(output, "FeatureBody: %s\n", n->feature_name);
            print_indent(output, indent + 2);
            fprintf(output, "Declarations:\n");
            print_ast_internal((AstNode*)n->declarations, output, indent + 4);
            print_indent(output, indent + 2);
            fprintf(output, "Statements:\n");
            print_ast_internal((AstNode*)n->statements, output, indent + 4);
            break;
        }
        case NODE_TYPE_CLASS_DECL: {
            ClassNode *n = (ClassNode*)node;
            fprintf(output, "Class: %s\n", n->name);
            print_ast_internal((AstNode*)n->features, output, indent + 2);
            break;
        }
        default:
            fprintf(output, "Unknown node type: %d\n", node->type);
            break;
    }
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
            /* Liberar strings si es necesario */
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
            /* Liberar nombre de feature, declaraciones y sentencias */
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
    /* Wrapper público para liberar todo el AST */
    free_ast_internal(node);
}
