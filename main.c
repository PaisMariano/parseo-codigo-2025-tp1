#include <stdio.h>
#include "interpreter.h"
#include "ast.h"

// Declaraciones externas para funciones y variables de Flex/Bison
extern FILE *yyin;
extern int yyparse(AstNode **root);
extern void yyerror(AstNode **root, const char *s);

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        // Leer desde la entrada estándar si no se proporciona un archivo
        yyin = stdin;
    }

    AstNode *root = NULL;
    int parse_result = yyparse(&root);

    if (parse_result == 0 && root != NULL) {
        // Inicializar y usar la tabla de símbolos
        SymbolTable table;
        init_symbol_table(&table);
        eval_ast(root, &table); // Pasar la tabla a la evaluación
        free_ast(root);
    }

    if (yyin && yyin != stdin) {
        fclose(yyin);
    }

    return parse_result;
}
