%{
#include "ast.h"
#include "definiciones.h"
#include "parser.tab.h" 
using namespace std;
%}
%option yylineno

%%

"me"							return ME;
[ ] 							;
\t								;
\n								;
"true"							return TRUE;
"false"							return FALSE;
,								return COMA;
\.								return PUNTO;
:								return DOSPUNTOS;
\(								return PARABRE;
\)								return PARCIERRA;
\+								return SUMA;
\-								return RESTA;
\*								return MULT;
\/								return DIV;
%								return MOD;
\/\\							return CONJUNCION;
\\\/							return DISYUNCION;
~								return NEGACION;
\<								return MENOR;
\<=								return MENORIGUAL;
>								return MAYOR;
>=								return MAYORIGUAL;
=								return IGUAL;
"create"						return CREATE;
"char"							return CHAR;
"while"							return WHILE;
"bool"							return BOOL;
"if"							return IF;
"else"							return ELSE;
"int"							return INT;
"on"							return ON;
"end"							return END;
"execute"						return EXECUTE;
"activation"					return ACTIVATION;
"deactivation"					return DEACTIVATION;
"default"						return DEFAULT;
"bot"							return BOT;
"activate"						return ACTIVATE;
"deactivate"					return DEACTIVATE;
"advance"						return ADVANCE;
"read"							return READ;
"send"							return SEND;
"receive"						return RECEIVE;
"store"							return STORE;
"collect"						return COLLECT;
"drop"							return DROP;
"left"							return LEFT;
"right"							return RIGHT;
"up"							return UP;
"as"							return AS;
"down"							return DOWN;
'(.|\\n|\\t)' 					return CHARACTER;
[0-9]+|0+						{yylval.num = atoi(yytext); return number;}
[a-zA-Z][a-zA-Z0-9_]*			{yylval.str = yytext; return IDENTIFIER;}
\$\$.*							return COMENTARIO;
\$\-[^\-\$]*\-\$				return COMENTARIO;
[0-9]+[a-zA-Z0-9]+				;
.								;

%%

int yywrap(void){
	return 1;
}