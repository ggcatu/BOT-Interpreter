#include <stdio.h>
#include <iostream>
#include <string>
#include "definiciones.h"
#include "ast.h"
#include "parser.tab.h"

using namespace std;

extern int yylex();
extern int yylineno;
extern int yyleng;
extern char* yytext;
extern int yyparse();
extern void yyerror(const char *);
extern ArbolSintactico * root_ast;

string nToWord[] = {  "NULL", "", "", "", "Character", "True", "False", "Num",
					  "Ident", "Coma", "Punto", "DosPuntos", "ParAbre", "ParCierra",
					  "Suma", "Resta", "Mult", "Div", "Mod", "Conjuncion", "Disyuncion",
					  "Negacion", "Menor", "MenorIgual", "Mayor", "MayorIgual", "Igual",
					  "Create", "While", "Bool", "If", "Else", "Int", "On", "End", "Execute",
					  "Activation", "Deactivation", "Default", "Bot", "Activate", "Deactivate",
					  "Advance", "Read", "Send", "Recieve", "Store", "Collect", "Drop",
					  "Left", "Right", "Up", "Down", "Me", ""};

int main (void) { 
	yyparse();
	root_ast->imprimir(0);
	return 0;
}
