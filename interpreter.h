#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

#define MAX_SYMBOLS 100
#define MAX_CLASSES 10

struct SymbolTable;

/* Comentario general:
   - Define los tipos de valores en tiempo de ejecución y la estructura de la tabla de símbolos.
   - La tabla de símbolos puede tener un padre para modelar scope en métodos (encadenamiento).
   - También se mantiene una tabla global de definiciones de clase.
*/

// Tipos de valores en tiempo de ejecución
typedef enum {
    VAL_TYPE_INT,
    VAL_TYPE_REAL,
    VAL_TYPE_STRING,
    VAL_TYPE_VOID,
    VAL_TYPE_OBJECT,
    VAL_TYPE_NULL // Para variables declaradas pero no inicializadas
} ValueType;

/* RuntimeValue: representa un valor en ejecución (primitivo o referencia a objeto) */
typedef struct {
    ValueType type;
    union {
        int int_val;
        double real_val;
        char *string_val;
        struct SymbolTable* object_val; // Puntero a la tabla de símbolos del objeto
    } as;
} RuntimeValue;

/* Entrada en la tabla de símbolos:
   - name: nombre de la variable
   - value: valor actual
   - type_name: tipo declarado si existe (ej. "INTEGER" o nombre de clase) para resolver 'create'
*/
typedef struct {
    char *name;
    RuntimeValue value;
    char *type_name; // tipo declarado
} SymbolTableEntry;

/* Tabla de símbolos:
   - entries almacena variables/atributos locales o de objeto
   - parent apunta al scope externo (por ejemplo, el objeto al ejecutar un método)
   - owner_class_name señala si esta tabla representa un objeto de clase X
*/
typedef struct SymbolTable {
    SymbolTableEntry entries[MAX_SYMBOLS];
    int count;
    struct SymbolTable* parent;
    char* owner_class_name;
} SymbolTable;

/* Definición de clase: nombre y AST con features (atributos/métodos) */
typedef struct {
    char* name;
    StatementListNode* feature_list;
} ClassDefinition;

extern ClassDefinition class_table[MAX_CLASSES];
extern int class_count;

// --- Prototipos de funciones del intérprete ---

// Funciones de la tabla de símbolos
void init_symbol_table(SymbolTable *table);
void set_symbol(SymbolTable *table, const char *name, RuntimeValue value);
RuntimeValue get_symbol(SymbolTable *table, const char *name);
void declare_symbol(SymbolTable *table, const char *name, const char *type_name); // CAMBIO: acepta type_name
SymbolTableEntry* find_symbol_entry(SymbolTable *table, const char *name);

// Funciones de gestión de clases
void register_class(const char* name, StatementListNode* features);
ClassDefinition* find_class(const char* name);

// Función para imprimir valores
void print_value(RuntimeValue value);

// Nueva función para imprimir la tabla de símbolos
void print_symbol_table(SymbolTable *table, FILE *output);

// Función principal de evaluación del AST
RuntimeValue eval_ast(AstNode *node, SymbolTable *table);

#endif // INTERPRETER_H
