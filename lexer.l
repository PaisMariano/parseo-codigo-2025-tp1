%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylineno;
%}

%option noyywrap yylineno

%%
[ \t\r]+ { /* ignorar espacios y tabs */ }
\n { /* salto de línea: yylineno actualizado automáticamente */ }
"--".* { /* comentario hasta fin de línea: ignorar */ }

\"([^\"\n]|\\.)*\" { return TOKEN_STRING; }
[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? { return TOKEN_NUMBER; }

":=" { return TOKEN_ASSIGN; }
"<=" { return TOKEN_LE; }
">=" { return TOKEN_GE; }
"=" { return TOKEN_EQ; }
"<" { return TOKEN_LT; }
">" { return TOKEN_GT; }
"+" { return TOKEN_PLUS; }
"-" { return TOKEN_MINUS; }
"*" { return TOKEN_MULT; }
"/" { return TOKEN_DIV; }
"(" { return TOKEN_LPAREN; }
")" { return TOKEN_RPAREN; }
";" { return TOKEN_SEMI; }
"," { return TOKEN_COMMA; }
":" { return TOKEN_COLON; }
"\." { return TOKEN_DOT; }

"class" { return TOKEN_CLASS; }
"create" { return TOKEN_CREATE; }
"feature" { return TOKEN_FEATURE; }
"do" { return TOKEN_DO; }
"end" { return TOKEN_END; }
"if" { return TOKEN_IF; }
"then" { return TOKEN_THEN; }
"else" { return TOKEN_ELSE; }
"from" { return TOKEN_FROM; }
"until" { return TOKEN_UNTIL; }
"loop" { return TOKEN_LOOP; }


[A-Za-z_][A-Za-z0-9_]* { return TOKEN_IDENTIFIER; }
%%