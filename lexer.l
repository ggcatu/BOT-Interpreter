%{
#include "ast.h"
#include "definiciones.h"
#include "parser.tab.h" 

using namespace std;
vector<Token *> errors;
int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng; \
    yylval.str = strdup(yytext);
%}

%option yylineno

%%

"me"							return ME;
[ ] 							{ }
\t								{yycolumn+= 3;}
\n								{yycolumn = 1;}
\r 								{ }
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
"store"							return STORE;
"collect"						return COLLECT;
"drop"							return DROP;
"left"							return LEFT;
"right"							return RIGHT;
"up"							return UP;
"as"							return AS;
"down"							return DOWN;
'\\t'							{yylval.ch = '\t'; return CHARACTER;}
'\\n'							{yylval.ch = '\n'; return CHARACTER;}
'\\''							{yylval.ch = '\''; return CHARACTER;}
'.' 							{yylval.ch = yytext[1]; return CHARACTER;}
[0-9]+|0+						{yylval.num = atoi(yytext); return number;}
[a-zA-Z][a-zA-Z0-9_]*			{strcpy(yylval.str,yytext); return IDENTIFIER;}
\$\$.*							{ }
\$\-(\-[^$]|[^\-])*\-\$			{ }
[0-9]+[a-zA-Z0-9]+				;
.								{errors.push_back(new TokenError(0, yycolumn-1, yylineno, yytext));}

%%

int yywrap(void){
	return 1;
}