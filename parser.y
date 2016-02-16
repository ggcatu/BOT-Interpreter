%error-verbose
%{
#include <iostream>
#include "definiciones.h"
#include "ast.h"
using namespace std;

#define YYDEBUG 1

extern int yylex(void);
extern int yycolumn;
extern int yylineno;
extern char * yytext;
ArbolSintactico * root_ast;
bool error_sintactico = 0; 

void yyerror (char const *s) {
	error_sintactico = 1;
	cout << "Parse error:" << s << "\nFila: " << yylineno << "\n" << "Columna: " << yycolumn-1-strlen(yytext) << "\n" ; 
}

%}

%locations
%start S

%left IGUAL MENOR MAYOR
%left SUMA RESTA
%left MULT DIV MOD
%left CONJUNCION DISYUNCION
%right NEGACION
%right DOSPUNTOS ELSE END

// %precedence DOSPUNTOS
// %precedence ELSE
// %precedence END

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
%token AS ME

%token <num> number
%token <str> IDENTIFIER
%token <ch> CHARACTER
%token <arb> arbol
%token <boolean> TRUE FALSE

%type <arb> S decl exec instr instrs lDecs declaracion tipo lComp condicion expr exprArit exprLogic exprAlt instrRobot instrsRobot

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

lComp		: ON condicion DOSPUNTOS instrsRobot END 			{;}
			| lComp ON condicion DOSPUNTOS instrsRobot END  	{;}
			;

instrsRobot : instrRobot 										{$$ = new instruccion($1);}
			| instrsRobot instrRobot 							{$$ = new instruccion($1,$2);}
			;

instrRobot 	: COLLECT PUNTO										{$$ = new intr_robot(3);}
			| COLLECT AS decl PUNTO								{$$ = new intr_robot($3, 3);}
			| STORE expr PUNTO									{$$ = new intr_robot($2, 0);}
			| DROP expr PUNTO									{$$ = new intr_robot($2, 1);}
			| UP expr PUNTO										{$$ = new intr_movimiento($2,0);}
			| DOWN expr PUNTO									{$$ = new intr_movimiento($2,1);}
			| LEFT expr PUNTO									{$$ = new intr_movimiento($2,2);}
			| RIGHT expr PUNTO									{$$ = new intr_movimiento($2,3);}
			| READ PUNTO										{$$ = new intr_robot(4);}
			| READ AS decl PUNTO								{$$ = new intr_robot($3, 4);}
			| SEND PUNTO										{$$ = new intr_robot(5);}
			| RECEIVE PUNTO										{$$ = new intr_robot(6);}
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
			| IF exprAlt DOSPUNTOS instrs END					{$$ = new intr_guardia($2,$4,0);}
			| IF exprAlt DOSPUNTOS instrs ELSE instrs END		{$$ = new intr_guardia($2,$4,$6,1);}
			| WHILE exprAlt DOSPUNTOS instrs END				{$$ = new intr_guardia($2,$4,2);}
			;

expr		: exprArit											{$$ = $1;}
			| exprLogic											{$$ = $1;}
			| CHARACTER											{$$ = new character($1);}
			;

exprAlt		: exprLogic											{;}
			| IDENTIFIER										{$$ = new identificador($1);}
			| CHARACTER											{$$ = new character($1);}
			| ME												{$$ = new me();}
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
			| ME 												{$$ = new me();}
			;
