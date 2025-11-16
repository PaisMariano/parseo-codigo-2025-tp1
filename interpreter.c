// Reemplaza el contenido de interpreter.c con esto:
            #include <stdio.h>
            #include <stdlib.h>
            #include <string.h>
            #include "interpreter.h"
            #include "ast.h"
            #include "parser.tab.h" // <-- AÑADIR ESTA LÍNEA

            // --- Funciones para la Tabla de Símbolos ---
            void init_symbol_table(SymbolTable *table) {
                table->count = 0;
            }

            void set_symbol(SymbolTable *table, const char *name, RuntimeValue value) {
                for (int i = 0; i < table->count; i++) {
                    if (strcmp(table->entries[i].name, name) == 0) {
                        table->entries[i].value = value;
                        return;
                    }
                }
                if (table->count < MAX_SYMBOLS) {
                    table->entries[table->count].name = strdup(name);
                    table->entries[table->count].value = value;
                    table->count++;
                }
            }

            RuntimeValue get_symbol(SymbolTable *table, const char *name) {
                for (int i = 0; i < table->count; i++) {
                    if (strcmp(table->entries[i].name, name) == 0) {
                        return table->entries[i].value;
                    }
                }
                RuntimeValue void_val = { .type = VAL_TYPE_VOID };
                fprintf(stderr, "Error: Variable '%s' no definida.\n", name);
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

                        // Asumimos operaciones con enteros por simplicidad
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
                            printf("\n"); // Añadir salto de línea después de cada print
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

                    // --- AÑADIR ESTOS NUEVOS CASES ---
                    case NODE_TYPE_COMPARISON_EXPR: {
                        ComparisonExprNode *comp_node = (ComparisonExprNode*) node;
                        RuntimeValue left_val = eval_ast(comp_node->left, table);
                        RuntimeValue right_val = eval_ast(comp_node->right, table);

                        result.type = VAL_TYPE_INT; // Usamos 1 para verdadero, 0 para falso
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
                        // La condición es verdadera si no es 0
                        if (condition_val.as.int_val != 0) {
                            eval_ast((AstNode*)if_node->then_branch, table);
                        } else {
                            if (if_node->else_branch)
                                eval_ast((AstNode*)if_node->else_branch, table);
                        }
                        break;
                    }
                    // --- FIN DE LOS NUEVOS CASES ---

                    case NODE_TYPE_ARGUMENT_LIST:
                        // Este nodo se maneja dentro de PROCEDURE_CALL, no se evalúa directamente.
                        break;
                }
                return result;
            }
