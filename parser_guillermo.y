%{
#include <string>
#include <cstring>
void yyerror(char* str);
%}

%start S

%left "+"
%left "-"
%left "*"
%left "/"
%left "%"
%left "="
%left "<"
%left ">"
%left "<="
%left ">="
%left "/\\"
%left "\\/"
%left "~"

%union {string str}

%token <str> termStr

%type <str> S decl exec sec instr ids expr exprArit exprRel exprBool

%%

S			: exec 											{cout << "Me metí por la 1ra del comienzo\n";}
			| "create" decl exec 							{cout << "Me metí por la 2da del comienzo\n");}
			;

decl		: termStr decl									{;}
			| termStr										{cout << "Hubo decl\n"}
			;

exec		: "execute" sec "end"							{cout << "Es sec\n";}
			| "execute" instr "end"							{cout << "No es sec\n";}
			;

sec			: instr sec										{cout << "soy conjInstr\n";}
			| instr											{cout << "soy smplInstr\n";}
			;

instr		: "activate" ids								{cout << "soy un activate\n";}
			| "deactivate" ids								{cout << "soy un deactivate\n";}
			| "if" exprBool ":" instr "end"					{cout << "soy un if\n";}
			| "if" exprBool ":" instr "else" instr "end"	{cout << "soy un ifElse\n";}
			| "while" exprBool ":" instr "end"				{cout << "soy un while\n";}
			| "store" expr "."								{cout << "soy un store\n";}
			| "collect" "."									{cout << "soy un collect\n";}
			| "collect" "as" ids "."						{cout << "soy un collect as\n";}
			| "drop" expr "."								{cout << "soy un drop\n";}
			| "up" expr "."									{cout << "soy un up\n";}
			| "down" expr "."								{cout << "soy un down\n";}
			| "left" expr "."								{cout << "soy un left\n";}
			| "right" expr "."								{cout << "soy un right\n";}
			| "read" "."									{cout << "soy un read\n";}
			| "read" "as" ids "."							{cout << "soy un read as\n";}
			| "send" "."									{cout << "soy un send\n";}
			| "advance" ids									{cout << "soy un advance\n";}
			| "receive" "."									{cout << "soy un receive\n";}
			;

ids			: termStr "," ids								{cout << "somos ids\n";}
			| termStr										{cout << "soy un id\n";}
			;


expr		: exprArit										{cout << "soy exprArit\n";}
			| exprRel										{cout << "soy exprRel\n";}
			| exprBool										{cout << "soy exprBool"}
			;

exprBool	: exprBool "/\\" exprBool						{cout << "soy un AND\n";}
			| exprBool "\\/" exprBool						{cout << "soy un OR\n";}
			| "~" exprBool									{cout << "soy un NEG\n";}
			| exprRel										{cout << "soy exprArit\n"}
			| termStr										{cout << "soy un terminal\n";}
			;

exprArit	: exprArit "+" exprArit							{cout << "soy suma\n";}
			| exprArit "-" exprArit							{cout << "soy resta\n";}
			| exprArit "*" exprArit							{cout << "soy mult\n";}
			| exprArit "/" exprArit							{cout << "soy div\n";}
			| exprArit "%" exprArit							{cout << "soy mod\n";}
			| termStr										{cout << "soy un terminal\n";}
			;

exprRel		: exprArit "=" exprArit							{cout << "soy igual\n";}
			| exprArit "<" exprArit							{cout << "soy menor\n";}
			| exprArit ">" exprArit							{cout << "soy mayor";}
			| exprArit "<=" exprArit						{cout << "soy menorIgual\n";}
			| exprArit ">=" exprArit						{cout << "soy mayorIgual\n";}
			;

