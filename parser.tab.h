/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_CLASS = 258,
    TOKEN_CREATE = 259,
    TOKEN_FEATURE = 260,
    TOKEN_DO = 261,
    TOKEN_END = 262,
    TOKEN_IF = 263,
    TOKEN_THEN = 264,
    TOKEN_ELSE = 265,
    TOKEN_FROM = 266,
    TOKEN_UNTIL = 267,
    TOKEN_LOOP = 268,
    TOKEN_IDENTIFIER = 269,
    TOKEN_NUMBER = 270,
    TOKEN_STRING = 271,
    TOKEN_ASSIGN = 272,
    TOKEN_PLUS = 273,
    TOKEN_MINUS = 274,
    TOKEN_MULT = 275,
    TOKEN_DIV = 276,
    TOKEN_EQ = 277,
    TOKEN_LT = 278,
    TOKEN_GT = 279,
    TOKEN_LE = 280,
    TOKEN_GE = 281,
    TOKEN_LPAREN = 282,
    TOKEN_RPAREN = 283,
    TOKEN_SEMI = 284,
    TOKEN_COMMA = 285,
    TOKEN_COLON = 286,
    TOKEN_DOT = 287
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
