/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IGUAL = 258,
    MENOR = 259,
    MAYOR = 260,
    SUMA = 261,
    RESTA = 262,
    MULT = 263,
    DIV = 264,
    MOD = 265,
    CONJUNCION = 266,
    DISYUNCION = 267,
    NEGACION = 268,
    DOSPUNTOS = 269,
    ELSE = 270,
    ENTER = 271,
    MENORIGUAL = 272,
    MAYORIGUAL = 273,
    PARABRE = 274,
    PARCIERRA = 275,
    COMA = 276,
    PUNTO = 277,
    BOT = 278,
    CREATE = 279,
    EXECUTE = 280,
    ACTIVATE = 281,
    DEACTIVATE = 282,
    STORE = 283,
    COLLECT = 284,
    DROP = 285,
    ADVANCE = 286,
    ON = 287,
    ACTIVATION = 288,
    DEACTIVATION = 289,
    DEFAULT = 290,
    UP = 291,
    DOWN = 292,
    LEFT = 293,
    RIGHT = 294,
    READ = 295,
    SEND = 296,
    RECEIVE = 297,
    IF = 298,
    END = 299,
    WHILE = 300,
    INT = 301,
    BOOL = 302,
    CHAR = 303,
    AS = 304,
    number = 305,
    IDENTIFIER = 306,
    CHARACTER = 307,
    arbol = 308,
    TRUE = 309,
    FALSE = 310
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 25 "parser.y" /* yacc.c:1909  */
	
			int num; 
			bool boolean;
			char * str;
			char ch;
			ArbolSintactico * arb;
		

#line 119 "parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
