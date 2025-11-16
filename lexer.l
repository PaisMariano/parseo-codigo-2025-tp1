%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"          // <-- Mover aquí para definir AstNode
#include "parser.tab.h"

extern YYSTYPE yylval;
%}

%option noyywrap yylineno nounput noinput

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*

%%
[ \t\r]+          { /* Ignorar espacios en blanco */ }
\n                { /* Ignorar saltos de línea */ }
"--".*            { /* Ignorar comentarios */ }

"class"           { return TOKEN_CLASS; }
"feature"         { return TOKEN_FEATURE; }
"do"              { return TOKEN_DO; }
"end"             { return TOKEN_END; }
"if"              { return TOKEN_IF; }
"then"            { return TOKEN_THEN; }
"else"            { return TOKEN_ELSE; }
"from"            { return TOKEN_FROM; }
"until"           { return TOKEN_UNTIL; }
"loop"            { return TOKEN_LOOP; }
"local"           { return TOKEN_LOCAL; }

{DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)? {
    yylval.real_val = atof(yytext);
    return TOKEN_NUMBER_REAL;
}

{DIGIT}+ {
    yylval.int_val = atoi(yytext);
    return TOKEN_NUMBER_INT;
}

\"[^\"\n]*\" {
    yylval.string_val = strndup(yytext + 1, yyleng - 2);
    return TOKEN_STRING;
}

{ID} {
    yylval.string_val = strdup(yytext);
    return TOKEN_IDENTIFIER;
}

":="              { return TOKEN_ASSIGN; }
"<="              { return TOKEN_LE; }
">="              { return TOKEN_GE; }
"="               { return TOKEN_EQ; }
"<"               { return TOKEN_LT; }
">"               { return TOKEN_GT; }
"+"               { return TOKEN_PLUS; }
"-"               { return TOKEN_MINUS; }
"*"               { return TOKEN_MULT; }
"/"               { return TOKEN_DIV; }
"("               { return TOKEN_LPAREN; }
")"               { return TOKEN_RPAREN; }
";"               { return TOKEN_SEMI; }
":"               { return TOKEN_COLON; }
","               { return TOKEN_COMMA; }

.                 { fprintf(stderr, "Error léxico en línea %d: Caracter inesperado '%s'\n", yylineno, yytext); }

%%
