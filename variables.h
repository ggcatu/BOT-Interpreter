#ifndef VARIA_H
#define VARIA_H

class variable { 
	public: 
		int tipo;
		bool init;
		variable(int ty): tipo(ty), init(false){};
		variable(int ty, bool i): tipo(ty), init(i){};
};

class variable_int: public variable {
	public:
		int * valor;
		variable_int(int ty): variable(ty) {
			valor = new int(0);
		};
		variable_int(int ty, int * v, bool i): variable(ty,i), valor(v) {};
};

class variable_bool: public variable {
	public:
		bool * valor;
		variable_bool(int ty): variable(ty) {};
		variable_bool(int ty, bool * v, bool i): variable(ty,i), valor(v) {};
};

class variable_char: public variable {
	public:
		char * valor;
		variable_char(int ty): variable(ty) {};
		variable_char(int ty, char * v, bool i): variable(ty,i), valor(v) {};
};

#endif