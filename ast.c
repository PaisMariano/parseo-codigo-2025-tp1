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

// --- IMPLEMENTACIONES PARA DECLARACIONES ---

DeclarationListNode* create_declaration_list_node(char* name, DeclarationListNode* next) {
    DeclarationListNode* node = malloc(sizeof(DeclarationListNode));
    node->base.type = NODE_TYPE_DECLARATION_LIST;
    node->variable_name = name;
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

AstNode* create_feature_body_node(DeclarationListNode* decls, StatementListNode* stmts) {
    FeatureBodyNode* node = malloc(sizeof(FeatureBodyNode));
    node->base.type = NODE_TYPE_FEATURE_BODY;
    node->declarations = decls;
    node->statements = stmts;
    return (AstNode*)node;
}


// --- Implementaciones para imprimir y liberar el AST ---

#include <stdarg.h>

static void print_indent(FILE *output, int indent) {
    for (int i = 0; i < indent; ++i) fputc(' ', output);
}

static void print_ast_internal(AstNode *node, FILE *output, int indent) {
    if (!node) {
        print_indent(output, indent);
        fprintf(output, "(null)\n");
        return;
    }

    switch (node->type) {
        case NODE_TYPE_LITERAL: {
            LiteralNode *n = (LiteralNode*)node;
            print_indent(output, indent);
            switch (n->literal_type) {
                case LITERAL_TYPE_INT:
                    fprintf(output, "Literal(int): %d\n", n->value.int_val);
                    break;
                case LITERAL_TYPE_REAL:
                    fprintf(output, "Literal(real): %f\n", n->value.real_val);
                    break;
                case LITERAL_TYPE_STRING:
                    fprintf(output, "Literal(string): \"%s\"\n", n->value.string_val ? n->value.string_val : "");
                    break;
                default:
                    fprintf(output, "Literal(unknown)\n");
            }
            break;
        }

        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *n = (BinaryExprNode*)node;
            print_indent(output, indent);
            fprintf(output, "BinaryExpr('%c')\n", n->op);
            print_ast_internal(n->left, output, indent + 2);
            print_ast_internal(n->right, output, indent + 2);
            break;
        }

        case NODE_TYPE_COMPARISON_EXPR: {
            ComparisonExprNode *n = (ComparisonExprNode*)node;
            print_indent(output, indent);
            fprintf(output, "ComparisonExpr(op=%d)\n", n->op);
            print_ast_internal(n->left, output, indent + 2);
            print_ast_internal(n->right, output, indent + 2);
            break;
        }

        case NODE_TYPE_PROCEDURE_CALL: {
            ProcedureCallNode *n = (ProcedureCallNode*)node;
            print_indent(output, indent);
            fprintf(output, "ProcedureCall: %s()\n", n->name ? n->name : "<anon>");
            ArgumentListNode *arg = n->arguments;
            while (arg) {
                print_ast_internal(arg->argument, output, indent + 2);
                arg = arg->next;
            }
            break;
        }

        case NODE_TYPE_METHOD_CALL: {
            MethodCallNode *n = (MethodCallNode*)node;
            print_indent(output, indent);
            fprintf(output, "MethodCall: %s.%s()\n", n->object_name ? n->object_name : "<obj>", n->method_name ? n->method_name : "<m>");
            ArgumentListNode *arg = n->arguments;
            while (arg) {
                print_ast_internal(arg->argument, output, indent + 2);
                arg = arg->next;
            }
            break;
        }

        case NODE_TYPE_ATTRIBUTE_ACCESS: {
            AttributeAccessNode *n = (AttributeAccessNode*)node;
            print_indent(output, indent);
            fprintf(output, "AttributeAccess: %s.%s\n", n->object_name ? n->object_name : "<obj>", n->attribute_name ? n->attribute_name : "<attr>");
            break;
        }

        case NODE_TYPE_CREATE: {
            CreateNode *n = (CreateNode*)node;
            print_indent(output, indent);
            fprintf(output, "Create: %s\n", n->object_name ? n->object_name : "<obj>");
            break;
        }

        case NODE_TYPE_VARIABLE: {
            VariableNode *n = (VariableNode*)node;
            print_indent(output, indent);
            fprintf(output, "Variable: %s\n", n->name ? n->name : "<var>");
            break;
        }

        case NODE_TYPE_ASSIGN: {
            AssignNode *n = (AssignNode*)node;
            print_indent(output, indent);
            fprintf(output, "Assign:\n");
            print_ast_internal(n->target, output, indent + 2);
            print_ast_internal(n->expression, output, indent + 2);
            break;
        }

        case NODE_TYPE_IF: {
            IfNode *n = (IfNode*)node;
            print_indent(output, indent);
            fprintf(output, "If:\n");
            print_ast_internal(n->condition, output, indent + 2);
            print_indent(output, indent + 2);
            fprintf(output, "Then:\n");
            StatementListNode *s = n->then_branch;
            while (s) {
                print_ast_internal(s->statement, output, indent + 4);
                s = s->next;
            }
            if (n->else_branch) {
                print_indent(output, indent + 2);
                fprintf(output, "Else:\n");
                s = n->else_branch;
                while (s) {
                    print_ast_internal(s->statement, output, indent + 4);
                    s = s->next;
                }
            }
            break;
        }

        case NODE_TYPE_LOOP: {
            LoopNode *n = (LoopNode*)node;
            print_indent(output, indent);
            fprintf(output, "Loop:\n");
            if (n->initialization) {
                print_indent(output, indent + 2);
                fprintf(output, "Init:\n");
                StatementListNode *s = n->initialization;
                while (s) {
                    print_ast_internal(s->statement, output, indent + 4);
                    s = s->next;
                }
            }
            if (n->condition) {
                print_indent(output, indent + 2);
                fprintf(output, "Condition:\n");
                print_ast_internal(n->condition, output, indent + 4);
            }
            if (n->loop_body) {
                print_indent(output, indent + 2);
                fprintf(output, "Body:\n");
                StatementListNode *s = n->loop_body;
                while (s) {
                    print_ast_internal(s->statement, output, indent + 4);
                    s = s->next;
                }
            }
            break;
        }

        case NODE_TYPE_ARGUMENT_LIST: {
            ArgumentListNode *n = (ArgumentListNode*)node;
            print_indent(output, indent);
            fprintf(output, "ArgumentList:\n");
            ArgumentListNode *it = n;
            while (it) {
                print_ast_internal(it->argument, output, indent + 2);
                it = it->next;
            }
            break;
        }

        case NODE_TYPE_STATEMENT_LIST: {
            StatementListNode *n = (StatementListNode*)node;
            print_indent(output, indent);
            fprintf(output, "StatementList:\n");
            StatementListNode *it = n;
            while (it) {
                print_ast_internal(it->statement, output, indent + 2);
                it = it->next;
            }
            break;
        }

        case NODE_TYPE_DECLARATION_LIST: {
            DeclarationListNode *n = (DeclarationListNode*)node;
            print_indent(output, indent);
            fprintf(output, "DeclarationList:\n");
            DeclarationListNode *it = n;
            while (it) {
                print_indent(output, indent + 2);
                fprintf(output, "Var: %s\n", it->variable_name ? it->variable_name : "<var>");
                it = it->next;
            }
            break;
        }

        case NODE_TYPE_FEATURE_BODY: {
            FeatureBodyNode *n = (FeatureBodyNode*)node;
            print_indent(output, indent);
            fprintf(output, "FeatureBody:\n");
            if (n->declarations) {
                print_indent(output, indent + 2);
                fprintf(output, "Declarations:\n");
                print_ast_internal((AstNode*)n->declarations, output, indent + 4);
            }
            if (n->statements) {
                print_indent(output, indent + 2);
                fprintf(output, "Statements:\n");
                print_ast_internal((AstNode*)n->statements, output, indent + 4);
            }
            break;
        }

        default:
            print_indent(output, indent);
            fprintf(output, "Unknown node type: %d\n", node->type);
            break;
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
            free(n);
            break;
        }

        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *n = (BinaryExprNode*)node;
            free_ast_internal(n->left);
            free_ast_internal(n->right);
            free(n);
            break;
        }

        case NODE_TYPE_COMPARISON_EXPR: {
            ComparisonExprNode *n = (ComparisonExprNode*)node;
            free_ast_internal(n->left);
            free_ast_internal(n->right);
            free(n);
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
            free(n);
            break;
        }

        case NODE_TYPE_METHOD_CALL: {
            MethodCallNode *n = (MethodCallNode*)node;
            if (n->object_name) free(n->object_name);
            if (n->method_name) free(n->method_name);
            ArgumentListNode *arg = n->arguments;
            while (arg) {
                ArgumentListNode *next = arg->next;
                free_ast_internal(arg->argument);
                free(arg);
                arg = next;
            }
            free(n);
            break;
        }

        case NODE_TYPE_ATTRIBUTE_ACCESS: {
            AttributeAccessNode *n = (AttributeAccessNode*)node;
            if (n->object_name) free(n->object_name);
            if (n->attribute_name) free(n->attribute_name);
            free(n);
            break;
        }

        case NODE_TYPE_CREATE: {
            CreateNode *n = (CreateNode*)node;
            if (n->object_name) free(n->object_name);
            free(n);
            break;
        }

        case NODE_TYPE_VARIABLE: {
            VariableNode *n = (VariableNode*)node;
            if (n->name) free(n->name);
            free(n);
            break;
        }

        case NODE_TYPE_ASSIGN: {
            AssignNode *n = (AssignNode*)node;
            free_ast_internal(n->target);
            free_ast_internal(n->expression);
            free(n);
            break;
        }

        case NODE_TYPE_IF: {
            IfNode *n = (IfNode*)node;
            free_ast_internal(n->condition);
            StatementListNode *s = n->then_branch;
            while (s) {
                StatementListNode *next = s->next;
                free_ast_internal(s->statement);
                free(s);
                s = next;
            }
            s = n->else_branch;
            while (s) {
                StatementListNode *next = s->next;
                free_ast_internal(s->statement);
                free(s);
                s = next;
            }
            free(n);
            break;
        }

        case NODE_TYPE_LOOP: {
            LoopNode *n = (LoopNode*)node;
            StatementListNode *s = n->initialization;
            while (s) {
                StatementListNode *next = s->next;
                free_ast_internal(s->statement);
                free(s);
                s = next;
            }
            free_ast_internal(n->condition);
            s = n->loop_body;
            while (s) {
                StatementListNode *next = s->next;
                free_ast_internal(s->statement);
                free(s);
                s = next;
            }
            free(n);
            break;
        }

        case NODE_TYPE_ARGUMENT_LIST: {
            ArgumentListNode *n = (ArgumentListNode*)node;
            ArgumentListNode *it = n;
            while (it) {
                ArgumentListNode *next = it->next;
                free_ast_internal(it->argument);
                free(it);
                it = next;
            }
            break;
        }

        case NODE_TYPE_STATEMENT_LIST: {
            StatementListNode *n = (StatementListNode*)node;
            StatementListNode *it = n;
            while (it) {
                StatementListNode *next = it->next;
                free_ast_internal(it->statement);
                free(it);
                it = next;
            }
            break;
        }

        case NODE_TYPE_DECLARATION_LIST: {
            DeclarationListNode *n = (DeclarationListNode*)node;
            DeclarationListNode *it = n;
            while (it) {
                DeclarationListNode *next = it->next;
                if (it->variable_name) free(it->variable_name);
                free(it);
                it = next;
            }
            break;
        }

        case NODE_TYPE_FEATURE_BODY: {
            FeatureBodyNode *n = (FeatureBodyNode*)node;
            if (n->declarations) free_ast_internal((AstNode*)n->declarations);
            if (n->statements) free_ast_internal((AstNode*)n->statements);
            free(n);
            break;
        }

        default:
            free(node);
            break;
    }
}

void print_ast(AstNode *node, FILE *output) {
    print_ast_internal(node, output, 0);
}

void free_ast(AstNode *node) {
    free_ast_internal(node);
}
