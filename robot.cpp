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
				sprintf(error_strp,"Error comportamiento nulo.");
				throw error_strp;
			}
			working_bot = NULL;
			head_table = tmp;
		} else {
			sprintf(error_strp,"Error se esta activando un robot activo.");
			throw error_strp;
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
				sprintf(error_strp,"Error comportamiento nulo.");
				throw error_strp;
			}
			working_bot = NULL;
			head_table = tmp;
		} else {
			sprintf(error_strp,"Error se esta desactivando un robot desactivado.");
			throw error_strp;
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
					sprintf(error_strp,"Error, el robot no ejecuto ningun comportamiento.");
					throw error_strp;
				}
			} else {
				sprintf(error_strp,"Error comportamiento nulo.");
				throw error_strp;
			}
			working_bot = NULL;
			head_table = tmp;
		} else {
			sprintf(error_strp,"Error se esta avanzando un robot desactivado.");
			throw error_strp;
		}
};