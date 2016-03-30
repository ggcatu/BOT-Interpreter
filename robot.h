/* Definicion de robot*/
#ifndef ROBOT_H
#define ROBOT_H
#include "arb_def.h"
#include "tabla_simbolos.h"
#include "variables.h"

class Robot {
	public:
		int tipo ;
		ArbolSintactico * valor;
		ArbolSintactico * comportamientos;
		bool activated;
		tabla_simbolos * tabla;
		int posicion[2];

	Robot(int ty, tabla_simbolos * head);

	Robot(Robot * pe);

	void activate();

	void deactivate();

	bool advance();

};

#endif