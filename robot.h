/* Definicion de robot*/
#ifndef ROBOT_H
#define ROBOT_H
#include "arb_def.h"
#include "tabla_simbolos.h"
#include "variables.h"

class Robot {
	public:
		int tipo;
		ArbolSintactico * valor;
		ArbolSintactico * comportamientos;
		bool init;
		bool activated;
		tabla_simbolos * tabla;
	Robot(int ty, tabla_simbolos * head);

	void activate();

	void deactivate();

	bool advance();
};

#endif