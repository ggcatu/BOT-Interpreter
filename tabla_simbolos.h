#ifndef TSIMB_H
#define TSIMB_H
#include <stdio.h>
#include <iostream>
#include <map>
#include "variables.h"

/* Clase tabla de simbolos, implementada con una tabla de hash
	y un apuntador a su padre, para poder tener la estructura 
	jerarquica requerida para el proyecto
*/
	
using namespace std;
class tabla_simbolos {
	public:
		map<string,int> mapa;
		map<string,variable *> valores;
		tabla_simbolos * padre;
		tabla_simbolos(){padre = NULL;};
		void print() { 
			for(map<string, int>::const_iterator it = mapa.begin();
			    it != mapa.end(); ++it)
			{
			    cout << it->first.c_str() << " " << it->second << endl;
			}
		}
};

#endif