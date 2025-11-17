%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"

    extern char *yytext;
    extern int yylineno;
    int yylex(void);
    extern FILE *yyin;

    void yyerror(AstNode **root, const char *s);
    AstNode *root_node;
%}

%union {
    int int_val;
    double real_val;
    char *string_val;
    AstNode *node;
    struct ArgumentListNode *arg_list;
    struct StatementListNode *stmt_list;
}

%token <string_val> TOKEN_IDENTIFIER TOKEN_STRING
%token <int_val> TOKEN_NUMBER_INT
%token <real_val> TOKEN_NUMBER_REAL

%token TOKEN_CLASS TOKEN_FEATURE TOKEN_DO TOKEN_END TOKEN_IF TOKEN_THEN TOKEN_ELSE
%token TOKEN_FROM TOKEN_UNTIL TOKEN_LOOP TOKEN_LOCAL TOKEN_CREATE
%token TOKEN_ASSIGN TOKEN_LE TOKEN_GE TOKEN_EQ TOKEN_LT TOKEN_GT TOKEN_DOT
%token TOKEN_PLUS TOKEN_MINUS TOKEN_MULT TOKEN_DIV
%token TOKEN_LPAREN TOKEN_RPAREN TOKEN_SEMI TOKEN_COLON TOKEN_COMMA

%nonassoc TOKEN_ASSIGN
%nonassoc TOKEN_LT TOKEN_GT TOKEN_LE TOKEN_GE TOKEN_EQ
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MULT TOKEN_DIV
%right TOKEN_DOT

%type <node> program class_declaration expression statement
%type <node> if_statement loop_statement feature_declaration create_statement
%type <node> primary_expression
%type <stmt_list> statement_list optional_statements feature_list
%type <arg_list> argument_list optional_argument_list

%parse-param { AstNode **root }

%%

program:
    class_declaration { *root = $1; }
    ;

class_declaration:
    TOKEN_CLASS TOKEN_IDENTIFIER TOKEN_FEATURE feature_list TOKEN_END { $$ = (AstNode*)$4; }
    ;

feature_list:
    feature_declaration { $$ = create_statement_list_node($1, NULL); }
    | feature_list feature_declaration { $$ = append_to_statement_list($1, $2); }
    ;

feature_declaration:
    TOKEN_IDENTIFIER formal_args local_clause TOKEN_DO statement_list TOKEN_END {
        /* Por ahora, el cuerpo de la función es la lista de sentencias */
        $$ = (AstNode*)$5;
    }
    ;

formal_args:
    TOKEN_LPAREN formal_arg_list TOKEN_RPAREN
    | /* empty */
    ;

formal_arg_list:
    /* empty */
    | identifier_list TOKEN_COLON type
    | formal_arg_list TOKEN_SEMI identifier_list TOKEN_COLON type
    ;

local_clause:
    TOKEN_LOCAL declarations
    | /* empty */
    ;

declarations:
    declaration_list
    ;

declaration_list:
    identifier_list TOKEN_COLON type
    | declaration_list TOKEN_SEMI identifier_list TOKEN_COLON type
    ;

identifier_list:
    TOKEN_IDENTIFIER { free($1); }
    | identifier_list TOKEN_COMMA TOKEN_IDENTIFIER { free($3); }
    ;

type:
    TOKEN_IDENTIFIER { free($1); }
    ;

statement_list:
    statement { $$ = create_statement_list_node($1, NULL); }
    | statement_list statement { $$ = append_to_statement_list($1, $2); }
    ;

optional_statements:
    /* empty */ { $$ = NULL; }
    | statement_list { $$ = $1; }
    ;

statement:
    primary_expression TOKEN_ASSIGN expression { $$ = create_assign_node(((VariableNode*)$1)->name, $3); }
    | primary_expression { $$ = $1; }
    | if_statement { $$ = $1; }
    | loop_statement { $$ = $1; }
    | create_statement { $$ = $1; }
    ;

create_statement:
    TOKEN_CREATE TOKEN_IDENTIFIER { $$ = create_create_node($2); }
    ;

if_statement:
    TOKEN_IF expression TOKEN_THEN optional_statements TOKEN_ELSE optional_statements TOKEN_END {
        $$ = create_if_node($2, $4, $6);
    }
    ;

loop_statement:
    TOKEN_FROM optional_statements TOKEN_UNTIL expression TOKEN_LOOP optional_statements TOKEN_END {
        $$ = create_loop_node($2, $4, $6);
    }
    ;

expression:
    primary_expression { $$ = $1; }
    | expression TOKEN_PLUS expression { $$ = create_binary_expr_node('+', $1, $3); }
    | expression TOKEN_MINUS expression { $$ = create_binary_expr_node('-', $1, $3); }
    | expression TOKEN_MULT expression { $$ = create_binary_expr_node('*', $1, $3); }
    | expression TOKEN_DIV expression { $$ = create_binary_expr_node('/', $1, $3); }
    | expression TOKEN_LT expression { $$ = create_comparison_expr_node(TOKEN_LT, $1, $3); }
    | expression TOKEN_GT expression { $$ = create_comparison_expr_node(TOKEN_GT, $1, $3); }
    | expression TOKEN_LE expression { $$ = create_comparison_expr_node(TOKEN_LE, $1, $3); }
    | expression TOKEN_GE expression { $$ = create_comparison_expr_node(TOKEN_GE, $1, $3); }
    | expression TOKEN_EQ expression { $$ = create_comparison_expr_node(TOKEN_EQ, $1, $3); }
    ;

primary_expression:
    TOKEN_NUMBER_INT { $$ = create_int_literal_node($1); }
    | TOKEN_NUMBER_REAL { $$ = create_real_literal_node($1); }
    | TOKEN_STRING { $$ = create_string_literal_node($1); }
    | TOKEN_IDENTIFIER { $$ = create_variable_node($1); }
    | TOKEN_LPAREN expression TOKEN_RPAREN { $$ = $2; }
    | TOKEN_IDENTIFIER TOKEN_LPAREN optional_argument_list TOKEN_RPAREN { $$ = create_procedure_call_node($1, $3); }
    | primary_expression TOKEN_DOT TOKEN_IDENTIFIER { $$ = create_attribute_access_node(((VariableNode*)$1)->name, $3); }
    | primary_expression TOKEN_DOT TOKEN_IDENTIFIER TOKEN_LPAREN optional_argument_list TOKEN_RPAREN { $$ = create_method_call_node(((VariableNode*)$1)->name, $3, $5); }
    ;

optional_argument_list:
    /* empty */ { $$ = NULL; }
    | argument_list { $$ = reverse_argument_list($1); }
    ;

argument_list:
    expression { $$ = create_argument_list_node($1, NULL); }
    | argument_list TOKEN_COMMA expression { $$ = create_argument_list_node($3, $1); }
    ;

%%
void yyerror(AstNode **root, const char *s) {
    fprintf(stderr, "Error de sintaxis en línea %d cerca de '%s': %s\n",
            yylineno, yytext, s);
}
