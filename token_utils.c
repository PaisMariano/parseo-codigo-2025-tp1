#include "token_utils.h"
#include <string.h>

const char* get_token_name(int token) {
    switch (token) {
        case TOKEN_CLASS: return "TOKEN_CLASS";
        case TOKEN_FEATURE: return "TOKEN_FEATURE";
        case TOKEN_DO: return "TOKEN_DO";
        case TOKEN_END: return "TOKEN_END";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_THEN: return "TOKEN_THEN";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_FROM: return "TOKEN_FROM";
        case TOKEN_UNTIL: return "TOKEN_UNTIL";
        case TOKEN_LOOP: return "TOKEN_LOOP";
        case TOKEN_LOCAL: return "TOKEN_LOCAL";
        case TOKEN_CREATE: return "TOKEN_CREATE";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_NUMBER_INT: return "TOKEN_NUMBER_INT";
        case TOKEN_NUMBER_REAL: return "TOKEN_NUMBER_REAL";
        case TOKEN_ASSIGN: return "TOKEN_ASSIGN";
        case TOKEN_LE: return "TOKEN_LE";
        case TOKEN_GE: return "TOKEN_GE";
        case TOKEN_EQ: return "TOKEN_EQ";
        case TOKEN_LT: return "TOKEN_LT";
        case TOKEN_GT: return "TOKEN_GT";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_MULT: return "TOKEN_MULT";
        case TOKEN_DIV: return "TOKEN_DIV";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_SEMI: return "TOKEN_SEMI";
        case TOKEN_COLON: return "TOKEN_COLON";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        default: return "UNKNOWN_TOKEN";
    }
}

void print_token(int token, YYSTYPE yylval, int yylineno) {
    if (!info_file_ptr) return;

    const char* token_name = get_token_name(token);
    fprintf(info_file_ptr, "[Line %d] %s: ", yylineno, token_name);

    switch (token) {
        case TOKEN_CLASS: fprintf(info_file_ptr, "\"class\"\n"); break;
        case TOKEN_FEATURE: fprintf(info_file_ptr, "\"feature\"\n"); break;
        case TOKEN_DO: fprintf(info_file_ptr, "\"do\"\n"); break;
        case TOKEN_END: fprintf(info_file_ptr, "\"end\"\n"); break;
        case TOKEN_IF: fprintf(info_file_ptr, "\"if\"\n"); break;
        case TOKEN_THEN: fprintf(info_file_ptr, "\"then\"\n"); break;
        case TOKEN_ELSE: fprintf(info_file_ptr, "\"else\"\n"); break;
        case TOKEN_FROM: fprintf(info_file_ptr, "\"from\"\n"); break;
        case TOKEN_UNTIL: fprintf(info_file_ptr, "\"until\"\n"); break;
        case TOKEN_LOOP: fprintf(info_file_ptr, "\"loop\"\n"); break;
        case TOKEN_LOCAL: fprintf(info_file_ptr, "\"local\"\n"); break;
        case TOKEN_CREATE: fprintf(info_file_ptr, "\"create\"\n"); break;
        case TOKEN_ASSIGN: fprintf(info_file_ptr, "\":=\"\n"); break;
        case TOKEN_LE: fprintf(info_file_ptr, "\"<=\"\n"); break;
        case TOKEN_GE: fprintf(info_file_ptr, "\">=\"\n"); break;
        case TOKEN_EQ: fprintf(info_file_ptr, "\"=\"\n"); break;
        case TOKEN_LT: fprintf(info_file_ptr, "\"<\"\n"); break;
        case TOKEN_GT: fprintf(info_file_ptr, "\">\"\n"); break;
        case TOKEN_DOT: fprintf(info_file_ptr, "\".\"\n"); break;
        case TOKEN_PLUS: fprintf(info_file_ptr, "\"+\"\n"); break;
        case TOKEN_MINUS: fprintf(info_file_ptr, "\"-\"\n"); break;
        case TOKEN_MULT: fprintf(info_file_ptr, "\"*\"\n"); break;
        case TOKEN_DIV: fprintf(info_file_ptr, "\"/\"\n"); break;
        case TOKEN_LPAREN: fprintf(info_file_ptr, "\"(\"\n"); break;
        case TOKEN_RPAREN: fprintf(info_file_ptr, "\")\"\n"); break;
        case TOKEN_SEMI: fprintf(info_file_ptr, "\";\"\n"); break;
        case TOKEN_COLON: fprintf(info_file_ptr, "\":\"\n"); break;
        case TOKEN_COMMA: fprintf(info_file_ptr, "\",\"\n"); break;

        case TOKEN_NUMBER_INT:
            fprintf(info_file_ptr, "%d\n", yylval.int_val);
            break;
        case TOKEN_NUMBER_REAL:
            fprintf(info_file_ptr, "%f\n", yylval.real_val);
            break;
        case TOKEN_STRING:
            fprintf(info_file_ptr, "\"%s\"\n", yylval.string_val);
            break;
        case TOKEN_IDENTIFIER:
            fprintf(info_file_ptr, "\"%s\"\n", yylval.string_val);
            break;
        default:
            fprintf(info_file_ptr, "\n");
            break;
    }
}

