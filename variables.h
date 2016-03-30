#ifndef VARIA_H
#define VARIA_H

class variable { 
	public: 
		int tipo;
		bool init;
		variable(int ty): tipo(ty), init(false){};
		variable(int ty, bool i): tipo(ty), init(i){};
		virtual variable * clone() {
			return new variable(tipo,init);
		}
};

class variable_int: public variable {
	public:
		int * valor;
		variable_int(int ty): variable(ty) {
			valor = new int(0);
		};
		variable_int(int ty, int * v, bool i): variable(ty,i), valor(v) {};
		virtual variable_int * clone(){
			return new variable_int(tipo, valor, init);
		}
};

class variable_bool: public variable {
	public:
		bool * valor;
		variable_bool(int ty): variable(ty) {};
		variable_bool(int ty, bool * v, bool i): variable(ty,i), valor(v) {};
		virtual variable_bool * clone(){
			return new variable_bool(tipo, valor, init);
		}
};

class variable_char: public variable {
	public:
		char * valor;
		variable_char(int ty): variable(ty) {};
		variable_char(int ty, char * v, bool i): variable(ty,i), valor(v) {};
		virtual variable_char * clone(){
			return new variable_char(tipo, valor, init);
		}
};

#endif