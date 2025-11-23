#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "ast.h"
#include "parser.tab.h"

// --- Tabla Global de Clases ---
ClassDefinition class_table[MAX_CLASSES];
int class_count = 0;

// --- Funciones para la Tabla de Símbolos ---
void init_symbol_table(SymbolTable *table) {
    table->count = 0;
    table->parent = NULL;
    table->owner_class_name = NULL;
}

SymbolTableEntry* find_symbol_entry(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->entries[i].name, name) == 0) {
            return &table->entries[i];
        }
    }
    return NULL;
}

void set_symbol(SymbolTable *table, const char *name, RuntimeValue value) {
    SymbolTableEntry* entry = find_symbol_entry(table, name);
    if (entry) {
        char* old_string_ptr = NULL;
        // Guardar el puntero del string antiguo si existe
        if (entry->value.type == VAL_TYPE_STRING && entry->value.as.string_val) {
            old_string_ptr = entry->value.as.string_val;
        }

        // Actualizar el valor en la tabla de símbolos
        entry->value = value;

        // Si había un string antiguo, y es diferente del nuevo, liberarlo.
        if (old_string_ptr) {
            char* new_string_ptr = (value.type == VAL_TYPE_STRING) ? value.as.string_val : NULL;
            if (old_string_ptr != new_string_ptr) {
                free(old_string_ptr);
            }
        }
    } else {
        if (table->count < MAX_SYMBOLS) {
            table->entries[table->count].name = strdup(name);
            table->entries[table->count].value = value;
            table->entries[table->count].type_name = NULL;
            table->count++;
        } else {
            fprintf(stderr, "Error: Tabla de símbolos llena.\n");
            exit(1);
        }
    }
}

RuntimeValue get_symbol(SymbolTable *table, const char *name) {
    // Caso especial para 'Current'
    if (strcmp(name, "Current") == 0) {
        SymbolTable* current_table = table;
        while(current_table && !current_table->owner_class_name) {
            current_table = current_table->parent;
        }
        if (current_table) {
            RuntimeValue obj_val;
            obj_val.type = VAL_TYPE_OBJECT;
            obj_val.as.object_val = current_table;
            return obj_val;
        }
    }

    SymbolTableEntry* entry = find_symbol_entry(table, name);
    if (entry) {
        return entry->value;
    }
    if (table->parent) {
        return get_symbol(table->parent, name);
    }

    fprintf(stderr, "Error: Variable '%s' no definida.\n", name);
    exit(1);
}

void declare_symbol(SymbolTable *table, const char *name, const char *type_name) {
    SymbolTableEntry* existing = find_symbol_entry(table, name);
    if (existing) {
        // Si ya existe, no re-declarar; pero si no tiene type_name y se provee, asignarlo.
        if (!existing->type_name && type_name) existing->type_name = strdup(type_name);
        return;
    }
    if (table->count < MAX_SYMBOLS) {
        RuntimeValue null_val = { .type = VAL_TYPE_NULL };
        table->entries[table->count].name = strdup(name);
        table->entries[table->count].value = null_val;
        table->entries[table->count].type_name = type_name ? strdup(type_name) : NULL;
        table->count++;
    }
}

// --- Funciones de Gestión de Clases ---
void register_class(const char* name, StatementListNode* features) {
    if (find_class(name) != NULL) return;
    if (class_count < MAX_CLASSES) {
        class_table[class_count].name = strdup(name);
        class_table[class_count].feature_list = features;
        class_count++;
    } else {
        fprintf(stderr, "Error: Demasiadas clases definidas.\n");
        exit(1);
    }
}

ClassDefinition* find_class(const char* name) {
    for (int i = 0; i < class_count; i++) {
        if (strcmp(class_table[i].name, name) == 0) {
            return &class_table[i];
        }
    }
    return NULL;
}


void print_value(RuntimeValue value) {
    switch (value.type) {
        case VAL_TYPE_INT:
            printf("%d", value.as.int_val);
            break;
        case VAL_TYPE_REAL:
            printf("%f", value.as.real_val);
            break;
        case VAL_TYPE_STRING:
            printf("%s", value.as.string_val);
            break;
        case VAL_TYPE_OBJECT:
            printf("[Object of class %s]", value.as.object_val->owner_class_name ? value.as.object_val->owner_class_name : "Unknown");
            break;
        case VAL_TYPE_VOID:
        case VAL_TYPE_NULL:
            break;
    }
}

// --- Nuevas funciones para imprimir la tabla de símbolos ---

// Imprime un valor en un stream específico (para depuración)
static void fprint_value(FILE* stream, RuntimeValue value) {
    switch (value.type) {
        case VAL_TYPE_INT:
            fprintf(stream, "%d", value.as.int_val);
            break;
        case VAL_TYPE_REAL:
            fprintf(stream, "%f", value.as.real_val);
            break;
        case VAL_TYPE_STRING:
            fprintf(stream, "\"%s\"", value.as.string_val);
            break;
        case VAL_TYPE_OBJECT:
            if (value.as.object_val) {
                fprintf(stream, "[Object of class %s at %p]", value.as.object_val->owner_class_name ? value.as.object_val->owner_class_name : "Unknown", (void*)value.as.object_val);
            } else {
                fprintf(stream, "[Object NULL]");
            }
            break;
        case VAL_TYPE_NULL:
            fprintf(stream, "NULL");
            break;
        case VAL_TYPE_VOID:
            fprintf(stream, "VOID");
            break;
    }
}

static void print_symbol_table_internal(SymbolTable *table, FILE *output, int indent) {
    if (!table) return;

    char indent_str[indent + 1];
    for(int i=0; i<indent; ++i) indent_str[i] = ' ';
    indent_str[indent] = '\0';

    fprintf(output, "%sSymbolTable at %p ", indent_str, (void*)table);
    if (table->owner_class_name) {
        fprintf(output, "(Class: %s)\n", table->owner_class_name);
    } else {
        fprintf(output, "(Scope)\n");
    }

    for (int i = 0; i < table->count; i++) {
        SymbolTableEntry *entry = &table->entries[i];
        fprintf(output, "%s  - %s (type: %s) = ", indent_str, entry->name, entry->type_name ? entry->type_name : "any");
        fprint_value(output, entry->value);
        fprintf(output, "\n");

        // Si el valor es un objeto, imprimir su tabla de símbolos de forma recursiva
        if (entry->value.type == VAL_TYPE_OBJECT && entry->value.as.object_val) {
            print_symbol_table_internal(entry->value.as.object_val, output, indent + 4);
        }
    }
}

void print_symbol_table(SymbolTable *table, FILE *output) {
    fprintf(output, "--- Symbol Table State ---\n");
    print_symbol_table_internal(table, output, 0);
    fprintf(output, "--------------------------\n");
}


// --- Función principal del Intérprete ---
RuntimeValue eval_ast(AstNode *node, SymbolTable *table) {
    RuntimeValue result = { .type = VAL_TYPE_VOID };
    if (!node) return result;

    switch (node->type) {
        case NODE_TYPE_LITERAL: {
            LiteralNode *n = (LiteralNode*)node;
            switch (n->literal_type) {
                case LITERAL_TYPE_INT:    result.type = VAL_TYPE_INT; result.as.int_val = n->value.int_val; break;
                case LITERAL_TYPE_REAL:   result.type = VAL_TYPE_REAL; result.as.real_val = n->value.real_val; break;
                case LITERAL_TYPE_STRING: result.type = VAL_TYPE_STRING; result.as.string_val = strdup(n->value.string_val); break;
            }
            break;
        }

        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *n = (BinaryExprNode*)node;
            RuntimeValue left = eval_ast(n->left, table);
            RuntimeValue right = eval_ast(n->right, table);

            // Lógica para enteros
            if (left.type == VAL_TYPE_INT && right.type == VAL_TYPE_INT) {
                result.type = VAL_TYPE_INT;
                switch (n->op) {
                    case '+': result.as.int_val = left.as.int_val + right.as.int_val; break;
                    case '-': result.as.int_val = left.as.int_val - right.as.int_val; break;
                    case '*': result.as.int_val = left.as.int_val * right.as.int_val; break;
                    case '/': result.as.int_val = left.as.int_val / right.as.int_val; break;
                }
            }
            // SOLUCIÓN: Añadir lógica para concatenación de strings
            else if (left.type == VAL_TYPE_STRING && right.type == VAL_TYPE_STRING && n->op == '+') {
                result.type = VAL_TYPE_STRING;
                size_t len1 = strlen(left.as.string_val);
                size_t len2 = strlen(right.as.string_val);
                char* new_str = malloc(len1 + len2 + 1);
                memcpy(new_str, left.as.string_val, len1);
                memcpy(new_str + len1, right.as.string_val, len2 + 1); // Copia el terminador nulo también
                result.as.string_val = new_str;
            }

            // Liberar memoria de operandos si eran strings temporales
            if (left.type == VAL_TYPE_STRING) free(left.as.string_val);
            if (right.type == VAL_TYPE_STRING) free(right.as.string_val);

            break;
        }

        case NODE_TYPE_PROCEDURE_CALL: {
            ProcedureCallNode *n = (ProcedureCallNode*)node;
            if (strcmp(n->name, "print") == 0) {
                ArgumentListNode *arg = n->arguments;
                while (arg) {
                    RuntimeValue val = eval_ast(arg->argument, table);
                    print_value(val);
                    arg = arg->next;
                }
                printf("\n");
            }
            break;
        }

        case NODE_TYPE_STATEMENT_LIST: {
            StatementListNode *list = (StatementListNode*)node;
            while (list) {
                eval_ast(list->statement, table);
                list = list->next;
            }
            break;
        }

        case NODE_TYPE_ASSIGN: {
            AssignNode *n = (AssignNode*)node;
            RuntimeValue value_to_assign = eval_ast(n->expression, table);

            if (n->target->type == NODE_TYPE_VARIABLE) {
                VariableNode *var_node = (VariableNode*)n->target;
                set_symbol(table, var_node->name, value_to_assign);
            } else if (n->target->type == NODE_TYPE_ATTRIBUTE_ACCESS) {
                AttributeAccessNode *attr_node = (AttributeAccessNode*)n->target;
                RuntimeValue object_val = eval_ast(attr_node->object_node, table);
                if (object_val.type == VAL_TYPE_OBJECT) {
                    set_symbol(object_val.as.object_val, attr_node->attribute_name, value_to_assign);
                }
            }

            // El valor temporal de la expresión ya fue consumido por set_symbol o liberado en su
            // respectivo case (ej. BINARY_EXPR). No se debe liberar aquí.
            // if (value_to_assign.type == VAL_TYPE_STRING) {
            //     free(value_to_assign.as.string_val);
            // }
            break;
        }

        case NODE_TYPE_VARIABLE: {
            VariableNode *n = (VariableNode*)node;
            result = get_symbol(table, n->name);
            // Si el valor es un string, duplicarlo para que el que llama sea dueño de la memoria.
            if (result.type == VAL_TYPE_STRING) {
                result.as.string_val = strdup(result.as.string_val);
            }
            break;
        }

        case NODE_TYPE_COMPARISON_EXPR: {
            ComparisonExprNode *n = (ComparisonExprNode*)node;
            RuntimeValue left = eval_ast(n->left, table);
            RuntimeValue right = eval_ast(n->right, table);
            result.type = VAL_TYPE_INT;
            int l = left.as.int_val, r = right.as.int_val;
            switch(n->op) {
                case TOKEN_LT: result.as.int_val = l < r; break;
                case TOKEN_LE: result.as.int_val = l <= r; break;
                case TOKEN_GT: result.as.int_val = l > r; break;
                case TOKEN_GE: result.as.int_val = l >= r; break;
                case TOKEN_EQ: result.as.int_val = l == r; break;
            }
            break;
        }

        case NODE_TYPE_IF: {
            IfNode *n = (IfNode*)node;
            RuntimeValue cond = eval_ast(n->condition, table);
            if (cond.as.int_val != 0) {
                eval_ast((AstNode*)n->then_branch, table);
            } else {
                eval_ast((AstNode*)n->else_branch, table);
            }
            break;
        }

        case NODE_TYPE_LOOP: {
            LoopNode *n = (LoopNode*)node;
            eval_ast((AstNode*)n->initialization, table);
            while(eval_ast(n->condition, table).as.int_val == 0) {
                eval_ast((AstNode*)n->loop_body, table);
            }
            break;
        }

        case NODE_TYPE_CREATE: {
            CreateNode *n = (CreateNode*)node;
            SymbolTableEntry* var_entry = find_symbol_entry(table, n->object_name);
            if (var_entry && var_entry->value.type == VAL_TYPE_NULL) {
                SymbolTable* new_object_table = malloc(sizeof(SymbolTable));
                init_symbol_table(new_object_table);

                // Determinar la clase a instanciar: si la variable tiene type_name en la tabla de símbolos la usamos
                const char* class_name_to_find = NULL;
                if (var_entry->type_name) {
                    class_name_to_find = var_entry->type_name;
                } else {
                    // Por compatibilidad, intentar COUNTER por defecto (antiguo comportamiento)
                    class_name_to_find = "COUNTER";
                }

                ClassDefinition* class_def = find_class(class_name_to_find);
                if (class_def) {
                    new_object_table->owner_class_name = strdup(class_def->name);
                    StatementListNode* feature = class_def->feature_list;
                    while(feature) {
                        if (feature->statement->type == NODE_TYPE_DECLARATION_LIST) {
                            DeclarationListNode* decl = (DeclarationListNode*)feature->statement;
                            while(decl) {
                                // Registrar atributo en la tabla del objeto con su tipo
                                declare_symbol(new_object_table, decl->variable_name, decl->type_name);
                                // Inicialización por defecto según tipo conocido
                                RuntimeValue init_val;
                                if (decl->type_name && strcmp(decl->type_name, "INTEGER") == 0) {
                                    init_val.type = VAL_TYPE_INT;
                                    init_val.as.int_val = 0;
                                    set_symbol(new_object_table, decl->variable_name, init_val);
                                } else if (decl->type_name && strcmp(decl->type_name, "REAL") == 0) {
                                    init_val.type = VAL_TYPE_REAL;
                                    init_val.as.real_val = 0.0;
                                    set_symbol(new_object_table, decl->variable_name, init_val);
                                } else if (decl->type_name && strcmp(decl->type_name, "STRING") == 0) {
                                    init_val.type = VAL_TYPE_STRING;
                                    init_val.as.string_val = strdup("");
                                    set_symbol(new_object_table, decl->variable_name, init_val);
                                } else {
                                    // Por defecto: dejar VAL_TYPE_NULL (referencias a objetos)
                                    // ya fue declarado como NULL por declare_symbol
                                }
                                decl = decl->next;
                            }
                        }
                        feature = feature->next;
                    }
                }

                RuntimeValue obj_val;
                obj_val.type = VAL_TYPE_OBJECT;
                obj_val.as.object_val = new_object_table;
                set_symbol(table, n->object_name, obj_val);
            }
            break;
        }

        case NODE_TYPE_ATTRIBUTE_ACCESS: {
            AttributeAccessNode *n = (AttributeAccessNode*)node;
            RuntimeValue object_val = eval_ast(n->object_node, table);
            if (object_val.type == VAL_TYPE_OBJECT) {
                SymbolTable* obj_table = object_val.as.object_val;
                ClassDefinition* class_def = find_class(obj_table->owner_class_name);

                // Buscar si es un método
                FeatureBodyNode* method_node = NULL;
                if (class_def) {
                    StatementListNode* feature = class_def->feature_list;
                    while(feature) {
                        if (feature->statement->type == NODE_TYPE_FEATURE_BODY) {
                            FeatureBodyNode* f_node = (FeatureBodyNode*)feature->statement;
                            if (f_node->feature_name && strcmp(f_node->feature_name, n->attribute_name) == 0) {
                                method_node = f_node;
                                break;
                            }
                        }
                        feature = feature->next;
                    }
                }

                if (method_node) { // Es una llamada a método sin argumentos
                    SymbolTable method_scope;
                    init_symbol_table(&method_scope);
                    method_scope.parent = obj_table;
                    eval_ast((AstNode*)method_node, &method_scope);
                    // Los métodos sin 'Result' devuelven void.
                    result.type = VAL_TYPE_VOID;
                } else { // Es un acceso a atributo
                    result = get_symbol(obj_table, n->attribute_name);
                }
            }
            break;
        }

        case NODE_TYPE_METHOD_CALL: {
            MethodCallNode *n = (MethodCallNode*)node;
            RuntimeValue object_val = eval_ast(n->object_node, table);
            if (object_val.type != VAL_TYPE_OBJECT) break;

            SymbolTable* obj_table = object_val.as.object_val;
            ClassDefinition* class_def = find_class(obj_table->owner_class_name);
            if (!class_def) break;

            StatementListNode* feature = class_def->feature_list;
            FeatureBodyNode* method_node = NULL;
            while(feature) {
                if (feature->statement->type == NODE_TYPE_FEATURE_BODY) {
                    FeatureBodyNode* f_node = (FeatureBodyNode*)feature->statement;
                    if (f_node->feature_name && strcmp(f_node->feature_name, n->method_name) == 0) {
                        method_node = f_node;
                        break;
                    }
                }
                feature = feature->next;
            }

            if (method_node) {
                SymbolTable method_scope;
                init_symbol_table(&method_scope);
                method_scope.parent = obj_table;
                eval_ast((AstNode*)method_node, &method_scope);
            }
            break;
        }

        case NODE_TYPE_FEATURE_BODY: {
            FeatureBodyNode *body_node = (FeatureBodyNode*) node;
            DeclarationListNode *decls = body_node->declarations;
            while (decls) {
                // Ahora pasamos el type_name al declarar variables locales / parámetros
                declare_symbol(table, decls->variable_name, decls->type_name);
                decls = decls->next;
            }
            eval_ast((AstNode*)body_node->statements, table);
            break;
        }

        case NODE_TYPE_CLASS_DECL:
            // Las clases se registran en main, no se evalúan directamente.
            break;

        case NODE_TYPE_DECLARATION_LIST:
        case NODE_TYPE_ARGUMENT_LIST:
            break;
    }
    return result;
}
