%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
extern int yylineno;
%}

%option noyywrap yylineno

%%
[ \t\r]+ { /* ignorar espacios y tabs */ }
\n { /* salto de línea: yylineno actualizado automáticamente */ }
"--".* { /* comentario hasta fin de línea: ignorar */ }

\"([^\"\n]|\\.)*\" { printf("TOKEN_STRING -> %s\n", yytext); return TOKEN_STRING; }
[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? { printf("TOKEN_NUMBER -> %s\n", yytext); return TOKEN_NUMBER; }

":=" { printf("TOKEN_ASSIGN -> %s\n", yytext); return TOKEN_ASSIGN; }
"<=" { printf("TOKEN_LE -> %s\n", yytext);  return TOKEN_LE; }
">=" { printf("TOKEN_GE -> %s\n", yytext);  return TOKEN_GE; }
"=" { printf("TOKEN_EQ -> %s\n", yytext); return TOKEN_EQ; }
"<" { printf("TOKEN_LT -> %s\n", yytext); return TOKEN_LT; }
">" { printf("TOKEN_GT -> %s\n", yytext); return TOKEN_GT; }
"+" { printf("TOKEN_PLUS -> %s\n", yytext); return TOKEN_PLUS; }
"-" { printf("TOKEN_MINUS -> %s\n", yytext); return TOKEN_MINUS; }
"*" { printf("TOKEN_MULT -> %s\n", yytext); return TOKEN_MULT; }
"/" { printf("TOKEN_DIV -> %s\n", yytext); return TOKEN_DIV; }
"(" { printf("TOKEN_LPAREN -> %s\n", yytext); return TOKEN_LPAREN; }
")" { printf("TOKEN_RPAREN -> %s\n", yytext); return TOKEN_RPAREN; }
";" { printf("TOKEN_SEMI -> %s\n", yytext); return TOKEN_SEMI; }
"," { printf("TOKEN_COMMA -> %s\n", yytext); return TOKEN_COMMA; }
":" { printf("TOKEN_COLON -> %s\n", yytext);return TOKEN_COLON; }
"\." { printf("TOKEN_DOT -> %s\n", yytext); return TOKEN_DOT; }

"class" { printf("TOKEN_CLASS -> %s\n", yytext); return TOKEN_CLASS; }
"create" { printf("TOKEN_CREATE -> %s\n", yytext); return TOKEN_CREATE; }
"feature" { printf("TOKEN_FEATURE -> %s\n", yytext); return TOKEN_FEATURE; }
"do" { printf("TOKEN_DO -> %s\n", yytext); return TOKEN_DO; }
"end" { printf("TOKEN_END -> %s\n", yytext); return TOKEN_END; }
"if" { printf("TOKEN_IF -> %s\n", yytext); return TOKEN_IF; }
"then" { printf("TOKEN_THEN -> %s\n", yytext); return TOKEN_THEN; }
"else" { printf("TOKEN_ELSE -> %s\n", yytext); return TOKEN_ELSE; }
"from" { printf("TOKEN_FROM -> %s\n", yytext); return TOKEN_FROM; }
"until" { printf("TOKEN_UNTIL -> %s\n", yytext); return TOKEN_UNTIL; }
"loop" { printf("TOKEN_LOOP -> %s\n", yytext); return TOKEN_LOOP; }


[A-Za-z_][A-Za-z0-9_]* { return TOKEN_IDENTIFIER; }

%%