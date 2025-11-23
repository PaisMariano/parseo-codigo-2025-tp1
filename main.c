#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "interpreter.h"

extern FILE *yyin;
extern int yyparse(AstNode **root);

FeatureBodyNode* find_feature(StatementListNode* feature_list, const char* feature_name);
void register_classes_from_ast(AstNode* node);

int main(int argc, char **argv) {
    FILE *info_file = NULL;

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }

        // Crear el archivo .info
        char info_filename[256];
        snprintf(info_filename, sizeof(info_filename), "%s.info", argv[1]);
        info_file = fopen(info_filename, "w");
        if (!info_file) {
            fprintf(stderr, "No se pudo crear el archivo de información %s.\n", info_filename);
            info_file = stderr; // Usar stderr como fallback
        }
    } else {
        info_file = stdout; // Si no hay archivo, usar stdout
    }

    AstNode *root = NULL;
    if (yyparse(&root) != 0) {
        fprintf(info_file, "Error de parseo.\n");
        if (info_file != stdout && info_file != stderr) fclose(info_file);
        return 1;
    }

    // Escribir el AST en el archivo .info
    fprintf(info_file, "--- AST Tree ---\n");
    print_ast(root, info_file);
    fprintf(info_file, "----------------\n\n");

    if (root == NULL) {
        if (info_file != stdout && info_file != stderr) fclose(info_file);
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

    free_ast(root);
    if (argc > 1) {
        fclose(yyin);
    }
    if (info_file != stdout && info_file != stderr) {
        fclose(info_file);
    }

    return 0;
}

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
