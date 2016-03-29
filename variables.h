#ifndef VARIA_H
#define VARIA_H

class variable { 
	public: 
		int tipo;
		bool init;
		variable(int ty): tipo(ty), init(false){};
};

class variable_int: public variable {
	public:
		int * valor;
		variable_int(int ty): variable(ty) {
			valor = new int(0);
		};
};

class variable_bool: public variable {
	public:
		bool * valor;
		variable_bool(int ty): variable(ty) {};
};

class variable_char: public variable {
	public:
		char * valor;
		variable_char(int ty): variable(ty) {};
};

#endif