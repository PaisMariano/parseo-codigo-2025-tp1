#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <stdio.h>
#include "ast.h" // Incluir ast.h ANTES de parser.tab.h
#include "parser.tab.h"

// Puntero global al archivo de salida .info
extern FILE *info_file_ptr;

// Prototipo de la función para obtener el nombre de un token
const char* get_token_name(int token);

// Prototipo de la función para imprimir un token
void print_token(int token, YYSTYPE yylval, int yylineno);

#endif // TOKEN_UTILS_H
