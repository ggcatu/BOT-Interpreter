/*
Traductores e Interpretadores (CI-3725)

Autores:
Guillermo Betancourt, carnet 11-10103
Gabriel Giménez, carnet 12-11006

definiciones.h:
Contiene un mapeo palabra -> integer para identificar los tipos de tokens, así como las firmas 
de las clases y funciones implementadas en definiciones.cpp
*/

#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdexcept>
#include "definiciones.h"
#include "ast.h"
#include "parser.tab.h"

using namespace std;

/*
	Funciones importadas de lex.yy.c, provenientes de FLEX.
	yylex permite leer el próximo token.
	yylineno indica la linea donde está ubicado el cursor.
	yyleng indica el largo del token.
	yytext es un apuntador al inicio del token actual.
	yyin es un apuntador al input stream de yylex.
*/

extern FILE* yyin;
extern int yyparse();
extern ArbolSintactico * root_ast;
extern vector<Token *> errors;
extern bool error_sintactico;

int main(int argc, char** argv) {

// Cambio del input stream al archivo en argv[1]
    yyin = fopen(argv[1],"r");
    if (yyin == false){
     	return 0;
    }

// Inicia parseo
	yyparse();

// Si hay errores del lexer, imprimirlos
	if (!errors.empty()){
		print_errors(errors);
	}
	
// Imprimir AST.
	if (!error_sintactico){
	   try {
		root_ast->imprimir(0);	
		}
		catch(int e){
			cout << "An exception occurred. Exception Nr. " << e << '\n';
		}
		catch(const char* const errorMessage){
			cout << errorMessage << endl;
		}
	}

	return 0;
}
