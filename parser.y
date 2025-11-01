%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "interpreter.h" // Incluye el intérprete

extern char *yytext;
extern int yylineno;
int yylex(void);
extern FILE *yyin;

void yyerror(const char *s);
AstNode *root; // Variable global para guardar la raíz del AST
%}

/* Definir los tipos de valores semánticos */
%union {
    int int_val;
    double real_val;
    char *string_val;
    AstNode *node; // Puntero a un nodo del AST
}

/* Asociar tipos a tokens y reglas no terminales */
%token <string_val> TOKEN_IDENTIFIER TOKEN_STRING
%token <int_val> TOKEN_NUMBER_INT
%token <real_val> TOKEN_NUMBER_REAL

%token TOKEN_CLASS TOKEN_FEATURE TOKEN_DO TOKEN_END
%token TOKEN_IF TOKEN_THEN TOKEN_ELSE TOKEN_FROM TOKEN_UNTIL TOKEN_LOOP
%token TOKEN_ASSIGN TOKEN_LE TOKEN_GE TOKEN_EQ TOKEN_LT TOKEN_GT
%token TOKEN_PLUS TOKEN_MINUS TOKEN_MULT TOKEN_DIV
%token TOKEN_LPAREN TOKEN_RPAREN TOKEN_SEMI TOKEN_COLON

/* Precedencia de operadores */
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MULT TOKEN_DIV

/* El tipo de las reglas que devuelven un puntero a un nodo del AST */
%type <node> program class_declaration feature_list feature_declaration
%type <node> statement_list statement assignment_statement if_statement
%type <node> expression term factor

%%

/* Reglas de la gramática y construcción del AST */

program:
    class_declaration { root = $1; }
    ;

class_declaration:
    TOKEN_CLASS TOKEN_IDENTIFIER TOKEN_FEATURE feature_list TOKEN_END {
        $$ = $4;
    }
    ;

feature_list:
    feature_declaration { $$ = $1; }
    ;

feature_declaration:
    TOKEN_IDENTIFIER TOKEN_DO statement_list TOKEN_END {
        $$ = $3;
    }
    ;

statement_list:
      statement { $$ = $1; }
    | statement_list TOKEN_SEMI statement {
        // Por simplicidad, solo se considera la última sentencia para la evaluación.
        // Una implementación real usaría una lista de nodos.
        $$ = $3;
    }
    ;

statement:
      assignment_statement { $$ = $1; }
    | if_statement         { $$ = $1; }
    | expression           { $$ = $1; } // Permitir expresiones como sentencias
    ;

assignment_statement:
    TOKEN_IDENTIFIER TOKEN_ASSIGN expression {
        // $$ = create_assignment_node($1, $3); // A implementar
    }
    ;

if_statement:
    TOKEN_IF expression TOKEN_THEN statement_list TOKEN_ELSE statement_list TOKEN_END {
        // $$ = create_if_node($2, $4, $6); // A implementar
    }
    ;

expression:
      term { $$ = $1; }
    | expression TOKEN_PLUS term { $$ = create_binary_expr_node('+', $1, $3); }
    | expression TOKEN_MINUS term { $$ = create_binary_expr_node('-', $1, $3); }
    ;

term:
      factor { $$ = $1; }
    | term TOKEN_MULT factor { $$ = create_binary_expr_node('*', $1, $3); }
    | term TOKEN_DIV factor { $$ = create_binary_expr_node('/', $1, $3); }
    ;

factor:
      TOKEN_NUMBER_INT { $$ = create_int_literal_node($1); }
    | TOKEN_NUMBER_REAL { $$ = create_real_literal_node($1); }
    | TOKEN_STRING { $$ = create_string_literal_node($1); }
    | TOKEN_IDENTIFIER { /* $$ = create_var_ref_node($1); */ }
    | TOKEN_LPAREN expression TOKEN_RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error de sintaxis en línea %d cerca de '%s': %s\n",
            yylineno, yytext, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    yyparse();

    if (root) {
        printf("AST construido. Evaluando...\n");
        RuntimeValue final_result = eval_ast(root);
        print_value(final_result);
        printf("\n");
        free_ast(root);
    }

    return 0;
}
