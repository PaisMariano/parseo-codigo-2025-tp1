#ifndef AST_H
#define AST_H

// Tipos de nodos del AST
typedef enum {
    NODE_TYPE_LITERAL,
    NODE_TYPE_BINARY_EXPR,
    // ... otros tipos de nodos (if, loop, etc.)
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
    AstNode base; // Miembro base para polimorfismo
    LiteralType literal_type;
    union {
        int int_val;
        double real_val;
        char *string_val;
    } value;
} LiteralNode;

// Nodo para expresiones binarias (ej. a + b)
typedef struct {
    AstNode base; // Miembro base para polimorfismo
    char op; // +, -, *, /
    struct AstNode *left;
    struct AstNode *right;
} BinaryExprNode;

// Funciones para crear nodos del AST
AstNode* create_binary_expr_node(char op, AstNode* left, AstNode* right);
AstNode* create_int_literal_node(int value);
AstNode* create_real_literal_node(double value);
AstNode* create_string_literal_node(char* value);

// Función para liberar la memoria del AST
void free_ast(AstNode *node);

#endif // AST_H
