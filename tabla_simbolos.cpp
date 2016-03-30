#include "robot.h"
#include "tabla_simbolos.h"


tabla_simbolos::tabla_simbolos(){
			padre = NULL;
};

tabla_simbolos::tabla_simbolos(tabla_simbolos * pe){
			padre = pe->padre;
			mapa = map<string,int>(pe->mapa);
			valores = map<string,variable *>(pe->valores);
			robots = map<string, Robot *>(pe->robots);
			//map<string,Robot *> robots ;
			for(map<string, Robot *>::const_iterator it = robots.begin();
			     it != robots.end(); ++it)
			 {
			 	robots[it->first]->posicion[0] = 0;
			 	robots[it->first]->posicion[1] = 0;
			 	robots[it->first]->activated = false;
			 }

};