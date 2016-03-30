#include "robot.h"
#include "tabla_simbolos.h"


tabla_simbolos::tabla_simbolos(){
			padre = NULL;
			robots = new map<string,Robot *>;
};