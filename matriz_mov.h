#ifndef MMOV_H
#define MMOV_H

#include "variables.h"

class superficie { 
	public: 
		map<int, map<int, variable *> > matriz;
		void agregar(int p1, int p2, variable * var){
			map[p1][p2] = var;
		}
};

#endif