#ifndef AST_H
#define AST_H

#include <stdio.h>

/* Comentario general:
   - Este header define los nodos del AST usados por el parser y el intérprete.
   - Cada nodo comienza con AstNode (campo 'type') para permitir dispatch por switch.
   - Hay nodos para literales, expresiones binarias, listas, llamadas, declaraciones y clases.
*/

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
    NODE_TYPE_LOOP,
    NODE_TYPE_ATTRIBUTE_ACCESS,
    NODE_TYPE_METHOD_CALL,
    NODE_TYPE_CREATE,
    NODE_TYPE_DECLARATION_LIST,
    NODE_TYPE_FEATURE_BODY,
    NODE_TYPE_CLASS_DECL // Nodo para una declaración de clase completa
} NodeType;

/* Tipos de literales:
   - INT, REAL y STRING se manejan con union en LiteralNode.
*/
typedef enum {
    LITERAL_TYPE_INT,
    LITERAL_TYPE_REAL,
    LITERAL_TYPE_STRING
} LiteralType;

/* Estructura base para nodos del AST */
typedef struct AstNode {
    NodeType type;
} AstNode;

/* Nodo para literales (números, strings) */
typedef struct {
    AstNode base;
    LiteralType literal_type;
    union {
        int int_val;
        double real_val;
        char *string_val;
    } value;
} LiteralNode;

/* Nodo para expresiones binarias (ej. a + b) */
typedef struct {
    AstNode base;
    char op;
    struct AstNode *left;
    struct AstNode *right;
} BinaryExprNode;

/* Nodo para expresiones de comparación (ej. a > b) */
typedef struct {
    AstNode base;
    int op;
    struct AstNode *left;
    struct AstNode *right;
} ComparisonExprNode;

/* Nodo para una lista de argumentos (encadenada) */
typedef struct ArgumentListNode {
    AstNode base;
    AstNode *argument;
    struct ArgumentListNode *next;
} ArgumentListNode;

/* Llamada a procedimiento global (ej. print("hola")) */
typedef struct {
    AstNode base;
    char *name;
    ArgumentListNode *arguments;
} ProcedureCallNode;

/* Lista de sentencias (cuerpo de métodos o bloques) */
typedef struct StatementListNode {
    AstNode base;
    AstNode *statement;
    struct StatementListNode *next;
} StatementListNode;

/* Nodo para if-then-else */
typedef struct {
    AstNode base;
    AstNode *condition;
    StatementListNode *then_branch;
    StatementListNode *else_branch;
} IfNode;

/* Asignación: target := expression
   - target puede ser VariableNode o AttributeAccessNode
*/
typedef struct {
    AstNode base;
    AstNode *target;
    AstNode *expression;
} AssignNode;

/* Uso de variable por nombre */
typedef struct {
    AstNode base;
    char *name;
} VariableNode;

/* Bucle from-until: initializations, condition, body */
typedef struct {
    AstNode base;
    StatementListNode *initialization;
    AstNode *condition;
    StatementListNode *loop_body;
} LoopNode;

/* Acceso a atributos: objeto.atributo */
typedef struct {
    AstNode base;
    AstNode *object_node;
    char *attribute_name;
} AttributeAccessNode;

/* Llamada a método: objeto.method(args) */
typedef struct {
    AstNode base;
    AstNode *object_node;
    char *method_name;
    ArgumentListNode *arguments;
} MethodCallNode;

/* 'create' crea una instancia y la asigna a un nombre */
typedef struct {
    AstNode base;
    char *object_name;
    char *class_name; // Nombre de clase asociado (si se resuelve)
} CreateNode;

/* Lista de declaraciones de variables (nombre + tipo opcional) */
typedef struct DeclarationListNode {
    AstNode base;
    char *variable_name;
    char *type_name; // Nombre del tipo (ej. "INTEGER" o nombre de clase)
    struct DeclarationListNode *next;
} DeclarationListNode;

/* Cuerpo de feature/método: nombre, declaraciones y sentencias */
typedef struct {
    AstNode base;
    char *feature_name; // Nombre del método/feature
    DeclarationListNode *declarations;
    StatementListNode *statements;
} FeatureBodyNode;

/* Nodo para declaración de clase:
   - name y lista de features (atributos y métodos)
*/
typedef struct {
    AstNode base;
    char *name;
    StatementListNode *features;
} ClassNode;

/* Prototipos: funciones para crear y manejar el AST (usadas por parser y main) */
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
AstNode* create_loop_node(StatementListNode* init, AstNode* condition, StatementListNode* body);
AstNode* create_create_node(char* obj_name);
ArgumentListNode* reverse_argument_list(ArgumentListNode* list);

AstNode* create_attribute_access_node(AstNode* obj_node, char* attr_name);
AstNode* create_method_call_node(AstNode* obj_node, char* method_name, ArgumentListNode* args);

DeclarationListNode* create_declaration_list_node(char* name, DeclarationListNode* next);
DeclarationListNode* append_to_declaration_list(DeclarationListNode* list, DeclarationListNode* new_decls);
void set_declaration_type(DeclarationListNode* list, char* type_name); // NUEVO
AstNode* create_feature_body_node(DeclarationListNode* decls, StatementListNode* stmts);
AstNode* create_class_node(char* name, StatementListNode* features); // Nuevo prototipo

void print_ast(AstNode *node, FILE *output);
void free_ast(AstNode *node);

#endif // AST_H
