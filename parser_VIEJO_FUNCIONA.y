%error-verbose
%{
#include <iostream>
#include "ast.h"
using namespace std;

int yylex(void);
void yyerror (char const *s) {
   cout << "Parse error:" << s << "\n"; 
}
ArbolSintactico * root_ast;
#define YYDEBUG 1
%}

%start S

%left IGUAL MENOR MAYOR
%left SUMA RESTA
%left MULT DIV MOD
%left CONJUNCION DISYUNCION
%nonassoc UMIN
%nonassoc TRY

%union {	
			int num; 
			bool boolean;
			char * str;
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
%token CHARACTER AS

%token <num> number
%token <str> IDENTIFIER
%token <arb> arbol
%token <boolean> TRUE FALSE


%type <arb> S decl exec instr instrs lDecs declaracion tipo lComp condicion expr exprArit exprRel exprBool exprLogic 

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
			| exprBool											{;}
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

			| STORE expr PUNTO									{cout << "soy un store\n";}
			| DROP expr PUNTO									{cout << "soy un drop\n";}

			| UP expr PUNTO										{$$ = new intr_movimiento($2,0);}
			| DOWN expr PUNTO									{$$ = new intr_movimiento($2,1);}
			| LEFT expr PUNTO									{$$ = new intr_movimiento($2,2);}
			| RIGHT expr PUNTO									{$$ = new intr_movimiento($2,3);}

			| SEND PUNTO										{cout << "soy un send\n";}
			| RECEIVE PUNTO										{cout << "soy un receive\n";}

			| IF exprBool DOSPUNTOS instr END					{cout << "soy un if\n";}
			| IF exprBool DOSPUNTOS instr ELSE instr END		{cout << "soy un ifElse\n";}
			| WHILE exprBool DOSPUNTOS instr END				{cout << "soy un while\n";}

expr		: exprArit											{$$ = $1;}
			| exprBool											{;}
			| CHARACTER											{;}
			| IDENTIFIER										{;}
			;

exprArit	: exprArit SUMA exprArit							{$$ = new expr_aritmetica($1,$3,0);}
			| exprArit RESTA exprArit 							{$$ = new expr_aritmetica($1,$3,1);}
			| exprArit MULT exprArit							{$$ = new expr_aritmetica($1,$3,2);}
			| exprArit DIV exprArit								{$$ = new expr_aritmetica($1,$3,3);}
			| exprArit MOD exprArit								{$$ = new expr_aritmetica($1,$3,4);}
			| PARABRE exprArit PARCIERRA						{$$ = new expr_aritmetica($2,5);}
			| RESTA exprArit 									{$$ = new expr_aritmetica($2,6);}
			| number											{$$ = new numero($1);}
			;

exprBool	: exprRel											{;}
			| exprLogic											{;}

exprRel		: exprArit IGUAL exprArit							{$$ = new expr_booleana($1,$3,0);}
			| exprArit MENOR exprArit							{$$ = new expr_booleana($1,$3,1);}
			| exprArit MAYOR exprArit							{$$ = new expr_booleana($1,$3,2);}
			| exprArit MENORIGUAL exprArit						{$$ = new expr_booleana($1,$3,3);}
			| exprArit MAYORIGUAL exprArit						{$$ = new expr_booleana($1,$3,4);}
			| PARABRE exprRel PARCIERRA							{$$ = new expr_booleana($2,5);}
			| TRUE												{$$ = new booleano($1);}
			| FALSE												{$$ = new booleano($1);}
			;

exprLogic	: exprLogic DISYUNCION exprLogic					{$$ = new expr_booleana($1,$3,6);}
			| exprLogic CONJUNCION exprLogic					{$$ = new expr_booleana($1,$3,7);}	
			| exprRel CONJUNCION exprRel						{$$ = new expr_booleana($1,$3,7);}
			| exprRel DISYUNCION exprRel						{$$ = new expr_booleana($1,$3,6);}
			| NEGACION exprRel									{$$ = new expr_booleana($2,8);}			
			;

