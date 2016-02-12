%error-verbose
%{
#include <iostream>
#include "definiciones.h"
#include "ast.h"
using namespace std;

int yylex(void);

ArbolSintactico * root_ast;

#define YYDEBUG 1

void yyerror (char const *s) {
   cout << "Parse error:" << s << "\n"; 
}

%}

%locations
%start S

%left IGUAL MENOR MAYOR
%left SUMA RESTA
%left MULT DIV MOD
%left CONJUNCION DISYUNCION
%right NEGACION
%precedence DOSPUNTOS
%precedence ELSE
%precedence END

%union {	
			int num; 
			bool boolean;
			char * str;
			char ch;
			ArbolSintactico * arb;
		}

%token ENTER
%token SUMA RESTA MULT DIV MOD
%token MENOR MAYOR IGUAL MENORIGUAL MAYORIGUAL
%token PARABRE PARCIERRA
%token CONJUNCION DISYUNCION NEGACION
%token COMA PUNTO DOSPUNTOS
%token BOT CREATE EXECUTE ACTIVATE DEACTIVATE STORE COLLECT DROP ADVANCE ON
%token ACTIVATION DEACTIVATION DEFAULT
%token UP DOWN LEFT RIGHT
%token READ SEND RECEIVE
%token IF ELSE END WHILE
%token INT BOOL CHAR
%token AS

%token <num> number
%token <str> IDENTIFIER
%token <ch> CHARACTER
%token <arb> arbol
%token <boolean> TRUE FALSE

%type <arb> S decl exec instr instrs lDecs declaracion tipo lComp condicion expr exprArit exprLogic

%%

S			: exec 												{$$ = new raiz($1); root_ast = new ArbolSintactico($$);}
			| CREATE lDecs exec 								{$$ = new raiz($3); root_ast = new ArbolSintactico($$);}
			;

lDecs 		: declaracion										{;}
			| lDecs declaracion									{;}
			;

declaracion : tipo BOT decl END 								{$$ = $3;}
			| tipo BOT decl lComp END 							{$$ = $3;}
			;

lComp		: ON condicion DOSPUNTOS instrs END 				{;}
			| lComp ON condicion DOSPUNTOS instrs END  			{;}
			;

decl		: IDENTIFIER										{$$ = new identificador($1);}
			| decl COMA IDENTIFIER 								{$$ = new instruccion($1,new identificador($3));}
			;

tipo 		: INT  												{;}
			| BOOL 												{;}
			| CHAR 												{;}

condicion  	: ACTIVATION										{;}
			| DEACTIVATION										{;}
			| DEFAULT											{;}
			| exprLogic											{$$ = $1;}
			;

exec		: EXECUTE instrs END								{$$ = $2;}
			;

instrs		: instr												{$$ = new instruccion($1);}
			| instrs instr 										{$$ = new instruccion($1,$2);}
			;

instr		: ACTIVATE decl	PUNTO								{$$ = new intr_robot($2, 0);}
			| DEACTIVATE decl PUNTO								{$$ = new intr_robot($2, 1);}
			| ADVANCE decl PUNTO								{$$ = new intr_robot($2, 2);}
			| COLLECT PUNTO										{$$ = new intr_robot(3);}
			| COLLECT AS decl PUNTO								{$$ = new intr_robot($3, 3);}
			| READ PUNTO										{$$ = new intr_robot(4);}
			| READ "as" decl PUNTO								{$$ = new intr_robot($3, 4);}
			| SEND PUNTO										{$$ = new intr_robot(5);}
			| RECEIVE PUNTO										{$$ = new intr_robot(6);}

			| STORE expr PUNTO									{$$ = new intr_robot($2, 0);}
			| DROP expr PUNTO									{$$ = new intr_robot($2, 1);}

			| UP expr PUNTO										{$$ = new intr_movimiento($2,0);}
			| DOWN expr PUNTO									{$$ = new intr_movimiento($2,1);}
			| LEFT expr PUNTO									{$$ = new intr_movimiento($2,2);}
			| RIGHT expr PUNTO									{$$ = new intr_movimiento($2,3);}

			| IF exprLogic DOSPUNTOS instrs END					{$$ = new intr_guardia($2,$4,0);}
			| IF exprLogic DOSPUNTOS instrs ELSE instrs END		{$$ = new intr_guardia($2,$4,$6,1);}
			| WHILE exprLogic DOSPUNTOS instrs END				{$$ = new intr_guardia($2,$4,2);}

expr		: exprArit											{$$ = $1;}
			| exprLogic											{$$ = $1;}
			| CHARACTER											{$$ = new character($1);}
			;

exprLogic	: exprArit IGUAL exprArit							{$$ = new expr_booleana($1,$3,0);}
			| exprArit MENOR exprArit							{$$ = new expr_booleana($1,$3,1);}
			| exprArit MAYOR exprArit							{$$ = new expr_booleana($1,$3,2);}
			| exprArit MENORIGUAL exprArit						{$$ = new expr_booleana($1,$3,3);}
			| exprArit MAYORIGUAL exprArit						{$$ = new expr_booleana($1,$3,4);}
			| PARABRE exprLogic PARCIERRA						{$$ = new expr_booleana($2,5);}
			| exprLogic DISYUNCION exprLogic					{$$ = new expr_booleana($1,$3,6);}
			| exprLogic CONJUNCION exprLogic					{$$ = new expr_booleana($1,$3,7);}
			| NEGACION exprLogic								{$$ = new expr_booleana($2,8);}	
			| TRUE												{$$ = new booleano(1);}
			| FALSE												{$$ = new booleano(0);}
			;

exprArit	: exprArit SUMA exprArit							{$$ = new expr_aritmetica($1,$3,0);}
			| exprArit RESTA exprArit 							{$$ = new expr_aritmetica($1,$3,1);}
			| exprArit MULT exprArit							{$$ = new expr_aritmetica($1,$3,2);}
			| exprArit DIV exprArit								{$$ = new expr_aritmetica($1,$3,3);}
			| exprArit MOD exprArit								{$$ = new expr_aritmetica($1,$3,4);}
			| PARABRE exprArit PARCIERRA						{$$ = new expr_aritmetica($2,5);}
			| RESTA exprArit 									{$$ = new expr_aritmetica($2,6);}
			| number											{$$ = new numero($1);}
			| IDENTIFIER										{$$ = new identificador($1);}
			;
