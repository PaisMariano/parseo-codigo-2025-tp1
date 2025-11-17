#ifndef AST_H
#define AST_H

#include <stdio.h>

// Tipos de nodos del AST
typedef enum {
    NODE_TYPE_LITERAL,
    NODE_TYPE_BINARY_EXPR,
    NODE_TYPE_PROCEDURE_CALL,
    NODE_TYPE_ARGUMENT_LIST,
    NODE_TYPE_STATEMENT_LIST,
    NODE_TYPE_ASSIGN,
    NODE_TYPE_VARIABLE,
    NODE_TYPE_IF,
    NODE_TYPE_COMPARISON_EXPR,
    // --- NUEVOS TIPOS ---
    NODE_TYPE_LOOP,
    NODE_TYPE_ATTRIBUTE_ACCESS,
    NODE_TYPE_METHOD_CALL,
    NODE_TYPE_CREATE
} NodeType;

// Tipos de literales
typedef enum {
    LITERAL_TYPE_INT,
    LITERAL_TYPE_REAL,
    LITERAL_TYPE_STRING
} LiteralType;

// Estructura base para todos los nodos del AST
typedef struct AstNode {
    NodeType type;
} AstNode;

// Nodo para literales (números, strings)
typedef struct {
    AstNode base;
    LiteralType literal_type;
    union {
        int int_val;
        double real_val;
        char *string_val;
    } value;
} LiteralNode;

// Nodo para expresiones binarias (ej. a + b)
typedef struct {
    AstNode base;
    char op;
    struct AstNode *left;
    struct AstNode *right;
} BinaryExprNode;

// Nodo para expresiones de comparación (ej. a > b)
typedef struct {
    AstNode base;
    int op; // Usamos int para tokens como TOKEN_GT, etc.
    struct AstNode *left;
    struct AstNode *right;
} ComparisonExprNode;

// Nodo para una lista de argumentos
typedef struct ArgumentListNode {
    AstNode base;
    AstNode *argument;
    struct ArgumentListNode *next;
} ArgumentListNode;

// Nodo para una llamada a procedimiento (ej. print("hola"))
typedef struct {
    AstNode base;
    char *name;
    ArgumentListNode *arguments;
} ProcedureCallNode;

// Nodo para una lista de sentencias (el cuerpo de un 'do')
typedef struct StatementListNode {
    AstNode base;
    AstNode *statement;
    struct StatementListNode *next;
} StatementListNode;

// Nodo para una sentencia if-then-else
typedef struct {
    AstNode base;
    AstNode *condition;
    StatementListNode *then_branch;
    StatementListNode *else_branch;
} IfNode;

// Nodo para una asignación (ej. x := 10 o c.value := 10)
typedef struct {
    AstNode base;
    AstNode *target; // Puede ser VariableNode o AttributeAccessNode
    AstNode *expression;
} AssignNode;

// Nodo para el uso de una variable (ej. print(x))
typedef struct {
    AstNode base;
    char *name;
} VariableNode;

// --- NUEVAS ESTRUCTURAS ---

// Nodo para un bucle from-until
typedef struct {
    AstNode base;
    StatementListNode *initialization;
    AstNode *condition;
    StatementListNode *loop_body;
} LoopNode;

// Nodo para acceso a atributos (ej. c.value)
typedef struct {
    AstNode base;
    char *object_name;
    char *attribute_name;
} AttributeAccessNode;

// Nodo para llamada a método (ej. c.inc)
typedef struct {
    AstNode base;
    char *object_name;
    char *method_name;
    ArgumentListNode *arguments;
} MethodCallNode;

// Nodo para 'create' (ej. create c)
typedef struct {
    AstNode base;
    char *object_name;
} CreateNode;


// Funciones para crear nodos del AST
AstNode* create_binary_expr_node(char op, AstNode* left, AstNode* right);
AstNode* create_int_literal_node(int value);
AstNode* create_real_literal_node(double value);
AstNode* create_string_literal_node(char* value);
AstNode* create_procedure_call_node(char* name, ArgumentListNode* args);
ArgumentListNode* create_argument_list_node(AstNode* arg, ArgumentListNode* next);
StatementListNode* create_statement_list_node(AstNode* stmt, StatementListNode* next);
StatementListNode* append_to_statement_list(StatementListNode* list, AstNode* stmt);
AstNode* create_assign_node(AstNode* target, AstNode* expr);
AstNode* create_variable_node(char* name);
AstNode* create_if_node(AstNode* condition, StatementListNode* then_branch, StatementListNode* else_branch);
AstNode* create_comparison_expr_node(int op, AstNode* left, AstNode* right);

// --- NUEVAS DECLARACIONES DE FUNCIONES ---
AstNode* create_loop_node(StatementListNode* init, AstNode* condition, StatementListNode* body);
AstNode* create_attribute_access_node(char* obj_name, char* attr_name);
AstNode* create_method_call_node(char* obj_name, char* method_name, ArgumentListNode* args);
AstNode* create_create_node(char* obj_name);
ArgumentListNode* reverse_argument_list(ArgumentListNode* list);


void print_ast(AstNode *node, FILE *output);
void free_ast(AstNode *node);

#endif // AST_H
