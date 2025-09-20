%{
#include <stdio.h>
#include <stdlib.h>
extern char *yytext;   /* provisto por Flex */
extern int yylineno;
int yylex(void);
extern FILE *yyin;

void yyerror(const char *s);
%}

%debug

%token TOKEN_CLASS TOKEN_CREATE TOKEN_FEATURE TOKEN_DO TOKEN_END
%token TOKEN_IF TOKEN_THEN TOKEN_ELSE TOKEN_FROM TOKEN_UNTIL TOKEN_LOOP
%token TOKEN_INHERIT TOKEN_CHECK TOKEN_REQUIRE TOKEN_ENSURE TOKEN_LOCAL
%token TOKEN_ALIAS TOKEN_IS TOKEN_WHEN
%token TOKEN_NOT TOKEN_AND TOKEN_OR
%token TOKEN_IDENTIFIER TOKEN_NUMBER TOKEN_STRING
%token TOKEN_ASSIGN TOKEN_LE TOKEN_GE TOKEN_EQ TOKEN_LT TOKEN_GT
%token TOKEN_PLUS TOKEN_MINUS TOKEN_MULT TOKEN_DIV
%token TOKEN_LPAREN TOKEN_RPAREN TOKEN_SEMI TOKEN_COMMA TOKEN_COLON TOKEN_DOT

%%
input:
      /* vacío */
    | input token
    ;

token:
      TOKEN_CLASS
    | TOKEN_CREATE
    | TOKEN_FEATURE
    | TOKEN_DO
    | TOKEN_END
    | TOKEN_IF
    | TOKEN_THEN
    | TOKEN_ELSE
    | TOKEN_FROM
    | TOKEN_UNTIL
    | TOKEN_LOOP
    | TOKEN_IDENTIFIER
    | TOKEN_NUMBER
    | TOKEN_STRING
    | TOKEN_ASSIGN
    | TOKEN_LE
    | TOKEN_GE
    | TOKEN_EQ
    | TOKEN_LT
    | TOKEN_GT
    | TOKEN_PLUS
    | TOKEN_MINUS
    | TOKEN_MULT
    | TOKEN_DIV
    | TOKEN_LPAREN
    | TOKEN_RPAREN
    | TOKEN_SEMI
    | TOKEN_COMMA
    | TOKEN_COLON
    | TOKEN_DOT
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de sintaxis en línea %d cerca de '%s': %s\n",
            yylineno, yytext, s);
}

const char* token_name(int tok) {
    switch(tok) {
        case TOKEN_CLASS: return "TOKEN_CLASS";
        case TOKEN_CREATE: return "TOKEN_CREATE";
        case TOKEN_FEATURE: return "TOKEN_FEATURE";
        case TOKEN_DO: return "TOKEN_DO";
        case TOKEN_END: return "TOKEN_END";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_THEN: return "TOKEN_THEN";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_FROM: return "TOKEN_FROM";
        case TOKEN_UNTIL: return "TOKEN_UNTIL";
        case TOKEN_LOOP: return "TOKEN_LOOP";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_ASSIGN: return "TOKEN_ASSIGN";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_MULT: return "TOKEN_MULT";
        case TOKEN_DIV: return "TOKEN_DIV";
        case TOKEN_EQ: return "TOKEN_EQ";
        case TOKEN_LT: return "TOKEN_LT";
        case TOKEN_GT: return "TOKEN_GT";
        case TOKEN_LE: return "TOKEN_LE";
        case TOKEN_GE: return "TOKEN_GE";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_SEMI: return "TOKEN_SEMI";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_COLON: return "TOKEN_COLON";
        case TOKEN_DOT: return "TOKEN_DOT";
        default: return "TOKEN_UNKNOWN";
    }
}

int main(int argc, char **argv) {
    if (argc > 1) yyin = fopen(argv[1], "r");
    yyparse();
    return 0;
}
