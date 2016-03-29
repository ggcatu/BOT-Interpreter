/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DISYUNCION = 258,
     CONJUNCION = 259,
     MENORIGUAL = 260,
     MAYORIGUAL = 261,
     MAYOR = 262,
     MENOR = 263,
     IGUAL = 264,
     RESTA = 265,
     SUMA = 266,
     MOD = 267,
     DIV = 268,
     MULT = 269,
     NEGACION = 270,
     END = 271,
     ELSE = 272,
     DOSPUNTOS = 273,
     ENTER = 274,
     PARABRE = 275,
     PARCIERRA = 276,
     COMA = 277,
     PUNTO = 278,
     BOT = 279,
     CREATE = 280,
     EXECUTE = 281,
     ACTIVATE = 282,
     DEACTIVATE = 283,
     STORE = 284,
     COLLECT = 285,
     DROP = 286,
     ADVANCE = 287,
     ON = 288,
     ACTIVATION = 289,
     DEACTIVATION = 290,
     DEFAULT = 291,
     UP = 292,
     DOWN = 293,
     LEFT = 294,
     RIGHT = 295,
     READ = 296,
     SEND = 297,
     RECEIVE = 298,
     IF = 299,
     WHILE = 300,
     INT = 301,
     BOOL = 302,
     CHAR = 303,
     AS = 304,
     ME = 305,
     number = 306,
     IDENTIFIER = 307,
     CHARACTER = 308,
     arbol = 309,
     TRUE = 310,
     FALSE = 311
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 38 "parser.y"
	
			int num; 
			bool boolean;
			char * str;
			char ch;
			ArbolSintactico * arb;
		

/* Line 2058 of yacc.c  */
#line 122 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
