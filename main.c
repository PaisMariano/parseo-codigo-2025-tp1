#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "interpreter.h"
#include "token_utils.h"

extern FILE *yyin;
extern int yyparse(AstNode **root);

FeatureBodyNode* find_feature(StatementListNode* feature_list, const char* feature_name);
void register_classes_from_ast(AstNode* node);

FILE *info_file_ptr = NULL;

/* Comentario:
   - main.c orquesta el proceso: abre el archivo, crea un .info para tokens/AST,
     parsea con Bison, registra clases, y ejecuta MAIN.make si existe.
   - Al final escribe el estado de la tabla de símbolos en el archivo .info para depuración.
*/

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }

        // Crear el archivo .info
        char info_filename[256];
        snprintf(info_filename, sizeof(info_filename), "%s.info", argv[1]);
        info_file_ptr = fopen(info_filename, "w");
        if (!info_file_ptr) {
            fprintf(stderr, "No se pudo crear el archivo de información %s.\n", info_filename);
            info_file_ptr = stderr; // Usar stderr como fallback
        }
    } else {
        info_file_ptr = stdout; // Si no hay archivo, usar stdout
    }

    // Escribir la cabecera de tokens
    fprintf(info_file_ptr, "--- TOKENS ---\n");

    AstNode *root = NULL;
    if (yyparse(&root) != 0) {
        fprintf(info_file_ptr, "Error de parseo.\n");
        if (info_file_ptr != stdout && info_file_ptr != stderr) fclose(info_file_ptr);
        return 1;
    }

    // Escribir el AST en el archivo .info
    fprintf(info_file_ptr, "\n--- AST Tree ---\n");
    print_ast(root, info_file_ptr);
    fprintf(info_file_ptr, "----------------\n\n");

    if (root == NULL) {
        if (info_file_ptr != stdout && info_file_ptr != stderr) fclose(info_file_ptr);
        return 0; // Archivo vacío, no es un error.
    }

    // Fase 1: Registrar todas las clases del AST
    register_classes_from_ast(root);

    SymbolTable global_scope;
    init_symbol_table(&global_scope);

    // Fase 2: Intentar ejecutar MAIN.make
    ClassDefinition* main_class = find_class("MAIN");
    FeatureBodyNode* make_method = NULL;
    if (main_class) {
        make_method = find_feature(main_class->feature_list, "make");
    }

    if (main_class && make_method) {
        // Si existe MAIN y make, lo ejecutamos
        eval_ast((AstNode*)make_method, &global_scope);
    } else {
        // Si no, ejecutamos el AST desde la raíz (para tests antiguos)
        eval_ast(root, &global_scope);
    }

    // Escribir el estado final de la tabla de símbolos en el archivo .info
    print_symbol_table(&global_scope, info_file_ptr);

    free_ast(root);
    if (argc > 1) {
        fclose(yyin);
    }
    if (info_file_ptr != stdout && info_file_ptr != stderr) {
        fclose(info_file_ptr);
    }

    return 0;
}

/* register_classes_from_ast:
   - Recorre la lista de sentencias en la raíz y registra las clases encontradas en class_table.
   - Esto separa la fase de parseo de la de ejecución.
*/
void register_classes_from_ast(AstNode* node) {
    if (!node || node->type != NODE_TYPE_STATEMENT_LIST) {
        return;
    }
    StatementListNode* list = (StatementListNode*)node;
    while (list) {
        if (list->statement && list->statement->type == NODE_TYPE_CLASS_DECL) {
            ClassNode* class_node = (ClassNode*)list->statement;
            register_class(class_node->name, class_node->features);
        }
        list = list->next;
    }
}

/* find_feature: busca un método por nombre dentro de la lista de features de una clase */
FeatureBodyNode* find_feature(StatementListNode* feature_list, const char* feature_name) {
    StatementListNode* current = feature_list;
    while (current) {
        AstNode* stmt = current->statement;
        if (stmt && stmt->type == NODE_TYPE_FEATURE_BODY) {
            FeatureBodyNode* f_node = (FeatureBodyNode*)stmt;
            if (f_node->feature_name && strcmp(f_node->feature_name, feature_name) == 0) {
                return f_node;
            }
        }
        current = current->next;
    }
    return NULL;
}
