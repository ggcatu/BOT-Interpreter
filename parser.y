%error-verbose
%{
#include <iostream>
#include "definiciones.h"
#include "ast.h"
#include <map>
using namespace std;

#define YYDEBUG 1

extern int yylex(void);
extern int yycolumn;
extern int yylineno;
extern char * yytext;
ArbolSintactico * root_ast;
tabla_simbolos * head_table = NULL;
tabla_simbolos * tmp_table = NULL;
map<string,Robot *>  robots;
bool error_sintactico = 0; 
void yyerror (char const *s) 
{	error_sintactico = 1;
	cout << "Parse error:" << s << "\nFila: " << yylineno << "\n" << "Columna: " << yycolumn-1-strlen(yytext) << "\n" ; 
}

%}

%locations
%start S

%left CONJUNCION DISYUNCION
%left IGUAL MENOR MAYOR MAYORIGUAL MENORIGUAL
%left SUMA RESTA
%left MULT DIV MOD
%right NEGACION
%right DOSPUNTOS ELSE END

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

%type <arb> S decl exec instr instrs lDecs declaracion tipo lComp condicion expr instrRobot instrsRobot lambda declaracionf

%%

S			: lambda exec 										{$$ = new raiz($2); root_ast = new ArbolSintactico($$);}
			| lambda CREATE lDecs exec 							{$$ = new raiz($3, $4); root_ast = new ArbolSintactico($$); 
																	static_cast<raiz *>($$)->table = *head_table; 
																	if (head_table->padre != NULL) {head_table = head_table->padre;} 
																}
			;
lambda		: {tmp_table = new tabla_simbolos(); tmp_table->padre = head_table; head_table = tmp_table;}
			;

lDecs 		: declaracion										{$$ = new instruccion($1);}
			| lDecs declaracion									{$$ = new instruccion($1,$2);}
			;

declaracion : declaracionf lComp END 							{$$ = $1; static_cast<declaracion *>($$)->add_comportamiento($2);head_table = head_table->padre;}
			| declaracionf END 									{$$ = $1; head_table = head_table->padre;}
			;

declaracionf: lambda tipo BOT decl 								{$$ = new declaracion($2,$4);}
			;

lComp		: ON condicion DOSPUNTOS instrsRobot END 			{$$ = new inside_bot($2,$4);}
			| lComp ON condicion DOSPUNTOS instrsRobot END  	{$$ = new instruccion($1,new inside_bot($3,$5));}
			;

instrsRobot : instrRobot 										{$$ = new instruccion($1);}
			| instrsRobot instrRobot 							{$$ = new instruccion($1,$2);}
			;

instrRobot 	: COLLECT PUNTO										{$$ = new intr_robot(3);}
			| COLLECT AS decl PUNTO								{$$ = new intr_robot($3, 3); $3->add_variable(head_table->mapa.at("me"),0);}

			| STORE expr PUNTO									{$$ = new intr_extra($2, 0);}
			| DROP expr PUNTO									{$$ = new intr_extra($2, 1);}


			| UP expr PUNTO										{$$ = new intr_movimiento($2,0);}
			| DOWN expr PUNTO									{$$ = new intr_movimiento($2,1);}
			| LEFT expr PUNTO									{$$ = new intr_movimiento($2,2);}
			| RIGHT expr PUNTO									{$$ = new intr_movimiento($2,3);}

			| UP PUNTO											{$$ = new intr_movimiento(0);}
			| DOWN PUNTO										{$$ = new intr_movimiento(1);}
			| LEFT PUNTO										{$$ = new intr_movimiento(2);}
			| RIGHT PUNTO										{$$ = new intr_movimiento(3);}


			| READ PUNTO										{$$ = new intr_robot(4);}

			| READ AS decl PUNTO								{$$ = new intr_robot($3, 4); $3->add_variable(head_table->mapa.at("me"),0);}
			| SEND PUNTO										{$$ = new intr_robot(5);}
			| RECEIVE PUNTO										{$$ = new intr_robot(6);}
			;

decl		: IDENTIFIER										{$$ = new identificador($1);}
			| decl COMA IDENTIFIER 								{$$ = new instruccion($1,new identificador($3));}
			;

tipo 		: INT  												{$$ = new numero();}
			| BOOL 												{$$ = new booleano();}
			| CHAR 												{$$ = new character();}

condicion  	: ACTIVATION										{$$ = new on_condicion(0);}
			| DEACTIVATION										{$$ = new on_condicion(1);}
			| DEFAULT											{$$ = new on_condicion(2);}
			| expr												{$$ = new on_condicion(3,$1);}
			;

exec		: EXECUTE instrs END								{$$ = $2;}
			;

instrs		: instr												{$$ = new instruccion($1);}
			| instrs instr 										{$$ = new instruccion($1,$2);}
			;

instr		: ACTIVATE decl	PUNTO								{$$ = new intr_robot($2, 0); $2->check();}
			| DEACTIVATE decl PUNTO								{$$ = new intr_robot($2, 1); $2->check();}
			| ADVANCE decl PUNTO								{$$ = new intr_robot($2, 2); $2->check();}

			| IF expr DOSPUNTOS instrs END						{$$ = new intr_guardia($2,$4,0);}
			| IF expr DOSPUNTOS instrs ELSE instrs END			{$$ = new intr_guardia($2,$4,$6,1);}
			| WHILE expr DOSPUNTOS instrs END					{$$ = new intr_guardia($2,$4,2);}
			| S 												{$$ = $1;}
			;


expr		: expr SUMA expr									{$$ = new expr_aritmetica($1,$3,0);}
			| expr RESTA expr		 							{$$ = new expr_aritmetica($1,$3,1);}
			| expr MULT expr									{$$ = new expr_aritmetica($1,$3,2);}
			| expr DIV expr										{$$ = new expr_aritmetica($1,$3,3);}
			| expr MOD expr										{$$ = new expr_aritmetica($1,$3,4);}
			| PARABRE expr PARCIERRA							{$$ = new expr_aritmetica($2,5);}
			| RESTA expr	 									{$$ = new expr_aritmetica($2,6);}
			| expr MENOR expr									{$$ = new expr_booleana($1,$3,1);}
			| expr MAYOR expr									{$$ = new expr_booleana($1,$3,2);}
			| expr MENORIGUAL expr								{$$ = new expr_booleana($1,$3,3);}
			| expr MAYORIGUAL expr								{$$ = new expr_booleana($1,$3,4);}
			| expr IGUAL expr									{$$ = new expr_booleana($1,$3,0);}	
			| expr DISYUNCION expr								{$$ = new expr_booleana($1,$3,6);}
			| expr CONJUNCION expr								{$$ = new expr_booleana($1,$3,7);}
			| NEGACION expr										{$$ = new expr_booleana($2,8);}	
			| TRUE												{$$ = new expr_booleana(new booleano(1),9);}
			| FALSE												{$$ = new expr_booleana(new booleano(0),9);}		
			| IDENTIFIER										{$$ = new identificador($1); $$->check();}
			| CHARACTER											{$$ = new character($1);}
			| number											{$$ = new expr_aritmetica(new numero($1), 7);}
			| ME 												{$$ = new me;}
			;
