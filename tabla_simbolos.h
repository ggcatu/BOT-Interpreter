#ifndef TSIMB_H
#define TSIMB_H
#include <stdio.h>
#include <iostream>
#include <map>
#include "robot.h"
#include "variables.h"


/* Clase tabla de simbolos, implementada con una tabla de hash
	y un apuntador a su padre, para poder tener la estructura 
	jerarquica requerida para el proyecto
*/
class Robot;

using namespace std;
class tabla_simbolos {
	public:
		map<string,int> mapa;
		map<string,variable *> valores;
		tabla_simbolos * padre;
		map<string,Robot *> * robots;
		tabla_simbolos();
		void print() { 
			for(map<string, int>::const_iterator it = mapa.begin();
			    it != mapa.end(); ++it)
			{
			    cout << it->first.c_str() << " " << it->second << endl;
			}
		}
		void print_val() { 
			for(map<string, variable *>::const_iterator it = valores.begin();
			    it != valores.end(); ++it)
			{
			    cout << it->first.c_str() << " " << it->second << endl;
			}
		}
};

#endif