%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
#include "token_utils.h"

extern YYSTYPE yylval;

/* Comentario: Este archivo define el analizador léxico (Flex).
   - Reconoce identificadores, números enteros y reales, strings, palabras reservadas y operadores.
   - Para cada token, usa print_token() para registrar información en el .info y luego devuelve el token al parser.
   - Las reglas están ordenadas de mayor a menor prioridad (strings y números antes que identificadores).
*/
%}

%option noyywrap yylineno nounput noinput

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*

%%
/* Reglas principales:
   - Ignoramos espacios, saltos y comentarios.
   - Palabras reservadas: "class", "feature", "do", ...
   - Números: se reconocen reales y enteros (orden importante).
   - Strings: se extrae el texto sin comillas.
   - Identificadores: se copian para que el parser cree nodos con el nombre.
   - Operadores y símbolos retornan su token correspondiente.
*/

[ \t\r]+          { /* Ignorar espacios en blanco */ }
\n                { /* Ignorar saltos de línea */ }
"--".*            { /* Ignorar comentarios */ }

"class"           { print_token(TOKEN_CLASS, yylval, yylineno); return TOKEN_CLASS; }
"feature"         { print_token(TOKEN_FEATURE, yylval, yylineno); return TOKEN_FEATURE; }
"do"              { print_token(TOKEN_DO, yylval, yylineno); return TOKEN_DO; }
"end"             { print_token(TOKEN_END, yylval, yylineno); return TOKEN_END; }
"if"              { print_token(TOKEN_IF, yylval, yylineno); return TOKEN_IF; }
"then"            { print_token(TOKEN_THEN, yylval, yylineno); return TOKEN_THEN; }
"else"            { print_token(TOKEN_ELSE, yylval, yylineno); return TOKEN_ELSE; }
"from"            { print_token(TOKEN_FROM, yylval, yylineno); return TOKEN_FROM; }
"until"           { print_token(TOKEN_UNTIL, yylval, yylineno); return TOKEN_UNTIL; }
"loop"            { print_token(TOKEN_LOOP, yylval, yylineno); return TOKEN_LOOP; }
"local"           { print_token(TOKEN_LOCAL, yylval, yylineno); return TOKEN_LOCAL; }
"create"          { print_token(TOKEN_CREATE, yylval, yylineno); return TOKEN_CREATE; }

{DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)? {
    yylval.real_val = atof(yytext);
    print_token(TOKEN_NUMBER_REAL, yylval, yylineno);
    return TOKEN_NUMBER_REAL;
}

{DIGIT}+ {
    yylval.int_val = atoi(yytext);
    print_token(TOKEN_NUMBER_INT, yylval, yylineno);
    return TOKEN_NUMBER_INT;
}

\"[^\"\n]*\" {
    yylval.string_val = strndup(yytext + 1, yyleng - 2);
    print_token(TOKEN_STRING, yylval, yylineno);
    return TOKEN_STRING;
}

{ID} {
    yylval.string_val = strdup(yytext);
    print_token(TOKEN_IDENTIFIER, yylval, yylineno);
    return TOKEN_IDENTIFIER;
}

":="              { print_token(TOKEN_ASSIGN, yylval, yylineno); return TOKEN_ASSIGN; }
"<="              { print_token(TOKEN_LE, yylval, yylineno); return TOKEN_LE; }
">="              { print_token(TOKEN_GE, yylval, yylineno); return TOKEN_GE; }
"="               { print_token(TOKEN_EQ, yylval, yylineno); return TOKEN_EQ; }
"<"               { print_token(TOKEN_LT, yylval, yylineno); return TOKEN_LT; }
">"               { print_token(TOKEN_GT, yylval, yylineno); return TOKEN_GT; }
"."               { print_token(TOKEN_DOT, yylval, yylineno); return TOKEN_DOT; }
"+"               { print_token(TOKEN_PLUS, yylval, yylineno); return TOKEN_PLUS; }
"-"               { print_token(TOKEN_MINUS, yylval, yylineno); return TOKEN_MINUS; }
"*"               { print_token(TOKEN_MULT, yylval, yylineno); return TOKEN_MULT; }
"/"               { print_token(TOKEN_DIV, yylval, yylineno); return TOKEN_DIV; }
"("               { print_token(TOKEN_LPAREN, yylval, yylineno); return TOKEN_LPAREN; }
")"               { print_token(TOKEN_RPAREN, yylval, yylineno); return TOKEN_RPAREN; }
";"               { print_token(TOKEN_SEMI, yylval, yylineno); return TOKEN_SEMI; }
":"               { print_token(TOKEN_COLON, yylval, yylineno); return TOKEN_COLON; }
","               { print_token(TOKEN_COMMA, yylval, yylineno); return TOKEN_COMMA; }

.                 { fprintf(stderr, "Error léxico en línea %d: Caracter inesperado '%s'\n", yylineno, yytext); }

%%
