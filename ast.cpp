#include "ast.h"
char error_strp[1000];

class variable { 
	public:
		string nombre;
		int tipo;
		variable(string n, int i): nombre(n), tipo(i) {};
};

class variable_int: public variable {
	public:
		int valor;
		variable_int(string n, int i): variable(n,i) {};
};

class variable_bool: public variable {
	public:
		bool valor;
		variable_bool(string n, int i): variable(n,i) {};
};
