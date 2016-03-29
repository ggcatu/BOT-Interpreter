#include "robot.h"
#include "arb_def.h"
#include "variables.h"
#include <stdio.h>
#include <iostream>

using namespace std;
extern tabla_simbolos * head_table;
extern char error_strp[1000];
extern Robot * working_bot;

Robot::Robot(int ty, tabla_simbolos * head){
		tipo = ty;
		tabla = head;
		activated = false;
		comportamientos = NULL;
		posicion[0] = 0;
		posicion[1] = 0;
}

void Robot::activate(){
		if (!activated){
			// Cambiar tabla de simbolos
			tabla_simbolos * tmp = head_table;
			head_table = tabla;
			// Colocar como robot activo
			working_bot = this;
			activated = true;
			if (comportamientos != NULL) {
				comportamientos -> activate();
			} else {
				cout << "ERROR COMPORTAMIENTO NULO" << endl;
			}
			head_table = tmp;
		} else {
			cout << "ERROR ESTAS ACTIVANDO 2 VECES" << endl;
		}
	}

void Robot::deactivate(){
		if (activated){
			tabla_simbolos * tmp = head_table;
			head_table = tabla;
			// Colocar como robot activo
			working_bot = this;
			activated = false;
			if (comportamientos != NULL) {
				comportamientos ->deactivate();
			} else {
				cout << "ERROR COMPORTAMIENTO NULO" << endl;
			}
			head_table = tmp;
		} else {
			cout << "ERROR ESTAS DESACTIVANDO DESACTIVADO" << endl;
		}
};

bool Robot::advance(){
		if (activated){
			tabla_simbolos * tmp = head_table;
			head_table = tabla;
			// Colocar como robot activo
			working_bot = this;
			if (comportamientos != NULL) {
				if (!comportamientos ->advance()){
					cout << "ERROR NINGUN COMPORTAMIENTO ACTIVADO" << endl;
				}
			} else {
				cout << "ERROR COMPORTAMIENTO NULO" << endl;
			}
			head_table = tmp;
		} else {
			cout << "ERROR ESTAS AVANZANDO DESACTIVADO" << endl;
		}
};