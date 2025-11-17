#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "ast.h"
#include "parser.tab.h"

// --- Funciones para la Tabla de Símbolos ---
void init_symbol_table(SymbolTable *table) {
    table->count = 0;
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
        // TODO: Liberar memoria del valor anterior si es necesario (ej. string)
        entry->value = value;
        return;
    }
    if (table->count < MAX_SYMBOLS) {
        table->entries[table->count].name = strdup(name);
        table->entries[table->count].value = value;
        table->count++;
    }
}

RuntimeValue get_symbol(SymbolTable *table, const char *name) {
    SymbolTableEntry* entry = find_symbol_entry(table, name);
    if (entry) {
        return entry->value;
    }
    RuntimeValue void_val = { .type = VAL_TYPE_VOID };
    fprintf(stderr, "Error: Variable '%s' no definida.\n", name);
    // exit(1); // Terminar en caso de error grave
    return void_val;
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
        case VAL_TYPE_VOID:
            // No imprimir nada para void
            break;
        case VAL_TYPE_OBJECT:
            printf("[Object]");
            break;
    }
}

// --- Función principal del Intérprete ---
RuntimeValue eval_ast(AstNode *node, SymbolTable *table) {
    RuntimeValue result = { .type = VAL_TYPE_VOID };
    if (!node) return result;

    switch (node->type) {
        case NODE_TYPE_LITERAL: {
            LiteralNode *lit_node = (LiteralNode*) node;
            if (lit_node->literal_type == LITERAL_TYPE_INT) {
                result.type = VAL_TYPE_INT;
                result.as.int_val = lit_node->value.int_val;
            } else if (lit_node->literal_type == LITERAL_TYPE_REAL) {
                result.type = VAL_TYPE_REAL;
                result.as.real_val = lit_node->value.real_val;
            } else if (lit_node->literal_type == LITERAL_TYPE_STRING) {
                result.type = VAL_TYPE_STRING;
                result.as.string_val = lit_node->value.string_val;
            }
            break;
        }

        case NODE_TYPE_BINARY_EXPR: {
            BinaryExprNode *bin_expr = (BinaryExprNode*) node;
            RuntimeValue left_val = eval_ast(bin_expr->left, table);
            RuntimeValue right_val = eval_ast(bin_expr->right, table);

            if (bin_expr->op == '+') {
                if (left_val.type == VAL_TYPE_STRING || right_val.type == VAL_TYPE_STRING) {
                    char l_str[100], r_str[100];
                    print_value(left_val); // Imprime a un buffer temporal
                    sprintf(l_str, "%s", (left_val.type == VAL_TYPE_INT) ? (sprintf(l_str, "%d", left_val.as.int_val), l_str) : left_val.as.string_val);
                    sprintf(r_str, "%s", (right_val.type == VAL_TYPE_INT) ? (sprintf(r_str, "%d", right_val.as.int_val), r_str) : right_val.as.string_val);

                    if (left_val.type == VAL_TYPE_INT) sprintf(l_str, "%d", left_val.as.int_val);
                    else if (left_val.type == VAL_TYPE_STRING) strcpy(l_str, left_val.as.string_val);

                    if (right_val.type == VAL_TYPE_INT) sprintf(r_str, "%d", right_val.as.int_val);
                    else if (right_val.type == VAL_TYPE_STRING) strcpy(r_str, right_val.as.string_val);

                    char* concat_str = malloc(strlen(l_str) + strlen(r_str) + 1);
                    strcpy(concat_str, l_str);
                    strcat(concat_str, r_str);
                    result.type = VAL_TYPE_STRING;
                    result.as.string_val = concat_str;
                    break;
                }
            }

            if (left_val.type == VAL_TYPE_INT && right_val.type == VAL_TYPE_INT) {
                result.type = VAL_TYPE_INT;
                switch (bin_expr->op) {
                    case '+': result.as.int_val = left_val.as.int_val + right_val.as.int_val; break;
                    case '-': result.as.int_val = left_val.as.int_val - right_val.as.int_val; break;
                    case '*': result.as.int_val = left_val.as.int_val * right_val.as.int_val; break;
                    case '/': result.as.int_val = left_val.as.int_val / right_val.as.int_val; break;
                }
            }
            break;
        }

        case NODE_TYPE_PROCEDURE_CALL: {
            ProcedureCallNode *proc_node = (ProcedureCallNode*) node;
            if (strcmp(proc_node->name, "print") == 0) {
                ArgumentListNode *arg_list = proc_node->arguments;
                while (arg_list != NULL) {
                    RuntimeValue arg_val = eval_ast(arg_list->argument, table);
                    print_value(arg_val);
                    arg_list = arg_list->next;
                }
                printf("\n");
            }
            break;
        }

        case NODE_TYPE_STATEMENT_LIST: {
            StatementListNode *list_node = (StatementListNode*) node;
            while (list_node != NULL) {
                eval_ast(list_node->statement, table);
                list_node = list_node->next;
            }
            break;
        }

        case NODE_TYPE_ASSIGN: {
            AssignNode *assign_node = (AssignNode*) node;
            RuntimeValue value_to_assign = eval_ast(assign_node->expression, table);
            set_symbol(table, assign_node->name, value_to_assign);
            break;
        }

        case NODE_TYPE_VARIABLE: {
            VariableNode *var_node = (VariableNode*) node;
            result = get_symbol(table, var_node->name);
            break;
        }

        case NODE_TYPE_COMPARISON_EXPR: {
            ComparisonExprNode *comp_node = (ComparisonExprNode*) node;
            RuntimeValue left_val = eval_ast(comp_node->left, table);
            RuntimeValue right_val = eval_ast(comp_node->right, table);

            result.type = VAL_TYPE_INT; // 1 para verdadero, 0 para falso
            int l = left_val.as.int_val;
            int r = right_val.as.int_val;

            switch (comp_node->op) {
                case TOKEN_GT: result.as.int_val = l > r; break;
                case TOKEN_LT: result.as.int_val = l < r; break;
                case TOKEN_GE: result.as.int_val = l >= r; break;
                case TOKEN_LE: result.as.int_val = l <= r; break;
                case TOKEN_EQ: result.as.int_val = l == r; break;
                default: result.as.int_val = 0; break;
            }
            break;
        }

        case NODE_TYPE_IF: {
            IfNode *if_node = (IfNode*) node;
            RuntimeValue condition_val = eval_ast(if_node->condition, table);
            if (condition_val.as.int_val != 0) {
                eval_ast((AstNode*)if_node->then_branch, table);
            } else {
                if (if_node->else_branch)
                    eval_ast((AstNode*)if_node->else_branch, table);
            }
            break;
        }

        // --- NUEVOS CASES ---
        case NODE_TYPE_LOOP: {
            LoopNode *loop_node = (LoopNode*) node;
            // Ejecutar la inicialización del bucle
            eval_ast((AstNode*)loop_node->initialization, table);
            // Bucle
            while (1) {
                RuntimeValue cond_val = eval_ast(loop_node->condition, table);
                if (cond_val.as.int_val != 0) { // La condición de 'until' es verdadera, salir
                    break;
                }
                // Ejecutar el cuerpo del bucle
                eval_ast((AstNode*)loop_node->loop_body, table);
            }
            break;
        }

        case NODE_TYPE_CREATE: {
            CreateNode *create_node = (CreateNode*) node;
            SymbolTable* new_object_table = malloc(sizeof(SymbolTable));
            init_symbol_table(new_object_table);

            RuntimeValue obj_val;
            obj_val.type = VAL_TYPE_OBJECT;
            obj_val.as.object_val = new_object_table;

            set_symbol(table, create_node->object_name, obj_val);
            break;
        }

        case NODE_TYPE_ATTRIBUTE_ACCESS: {
             // Este nodo solo debería aparecer dentro de una asignación o expresión.
             // La lógica se manejará en el nodo padre (ej. ASSIGN).
             // Aquí, simplemente lo evaluamos para obtener el valor.
            AttributeAccessNode *attr_node = (AttributeAccessNode*) node;
            RuntimeValue object_val = get_symbol(table, attr_node->object_name);
            if (object_val.type == VAL_TYPE_OBJECT) {
                result = get_symbol(object_val.as.object_val, attr_node->attribute_name);
            } else {
                 fprintf(stderr, "Error: '%s' no es un objeto.\n", attr_node->object_name);
            }
            break;
        }

        case NODE_TYPE_METHOD_CALL: {
            MethodCallNode *method_node = (MethodCallNode*) node;
            if (strcmp(method_node->method_name, "inc") == 0) {
                RuntimeValue counter_obj_val = get_symbol(table, method_node->object_name);
                if (counter_obj_val.type == VAL_TYPE_OBJECT) {
                    SymbolTable* obj_table = counter_obj_val.as.object_val;
                    RuntimeValue current_val = get_symbol(obj_table, "value");
                    if (current_val.type == VAL_TYPE_INT) {
                        current_val.as.int_val++;
                        set_symbol(obj_table, "value", current_val);
                    }
                }
            }
            // Aquí iría la lógica para otras llamadas a métodos
            break;
        }

        case NODE_TYPE_ARGUMENT_LIST:
            // Este nodo se maneja dentro de PROCEDURE_CALL, no se evalúa directamente.
            break;
    }
    return result;
}
