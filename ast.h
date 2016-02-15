#include <stdio.h>
#include <iostream>
using namespace std;


class ArbolSintactico {
	ArbolSintactico * first;
	public:
		ArbolSintactico(){};
		ArbolSintactico(ArbolSintactico * l): first(l) {};
		virtual void imprimir(int i){ if(first != NULL) first->imprimir(i); }
};

class raiz : public ArbolSintactico {
	public:
		ArbolSintactico * declaraciones;
		ArbolSintactico * ejecucion;
		raiz(ArbolSintactico * l) : ejecucion(l) {}
		virtual void imprimir(int i){
			if(ejecucion != NULL){
				cout << "SECUENCIACION:" << endl;
				ejecucion -> imprimir(1);
			}
		}
};

class instruccion : public ArbolSintactico {
	public:
		ArbolSintactico * left;
		ArbolSintactico * rigth;
		instruccion(ArbolSintactico * r) : rigth(r) {}
		instruccion(ArbolSintactico * l, ArbolSintactico * r) : left(l), rigth(r) {} 
		virtual void imprimir(int i){
			if (left != NULL ) {
				left -> imprimir(i);	
			}
			if (rigth != NULL ) {
				rigth -> imprimir(i);	
			}
		}
};


class intr_movimiento : public ArbolSintactico {
		enum mov { UP, DOWN, LEFT, RIGTH };
	public:
		ArbolSintactico * down;
		mov movimiento;
		intr_movimiento(ArbolSintactico * l, int m) : down(l), movimiento(static_cast<mov>(m)) {}
		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			switch(movimiento){
				case UP:
					cout << "UP:" << endl;
					break;
				case DOWN:
					cout << "DOWN:" << endl;
					break;
				case LEFT:
					cout << "LEFT:" << endl;
					break;
				case RIGTH:
					cout << "RIGTH:" << endl;
					break;
			}
			down -> imprimir(i + i);
		}
};

class intr_extra : public ArbolSintactico {
		enum mov { STORE, DROP };
	public:
		ArbolSintactico * down;
		mov movimiento;
		intr_extra(ArbolSintactico * l, int m) : down(l), movimiento(static_cast<mov>(m)) {}
		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			switch(movimiento)
{				case STORE:
					cout << "STORE:" << endl;
					break;
				case DROP:
					cout << "DROP:" << endl;
					break;
			}
			down -> imprimir(i + i);
		}
};

class intr_guardia : public ArbolSintactico {
		enum inst { IF, IF_ELSE, WHILE };
	public:
		ArbolSintactico * condicion;
		ArbolSintactico * cuerpo;
		ArbolSintactico * cuerpo_else;
		inst instruccion;
		intr_guardia(ArbolSintactico * l, ArbolSintactico * d, int m) : condicion(l), cuerpo(d), instruccion(static_cast<inst>(m)) {}
		intr_guardia(ArbolSintactico * l, ArbolSintactico * d, ArbolSintactico * ce, int m) : 
													   condicion(l), cuerpo(d), cuerpo_else(ce), instruccion(static_cast<inst>(m)) {}

		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			switch(instruccion){
				case IF:
					cout << "IF:" << endl;
					break;
				case IF_ELSE:
					cout << "IF:" << endl;
					break;
				case WHILE:
					cout << "WHILE:" << endl;
					break;
			}
			for (int j = 0; j < i; j++) cout << "	";
			cout << "GUARDIA: " << endl;
				condicion -> imprimir(i + i);
			for (int j = 0; j < i; j++) cout << "	";
			cout << "CUERPO: " << endl;
				cuerpo -> imprimir(i + i);
			if (cuerpo_else != NULL){
				for (int j = 0; j < i; j++) cout << "	";
					cout << "ELSE: " << endl;
					cuerpo_else -> imprimir(i + i);
			}
		}
};


class intr_robot : public ArbolSintactico {
		enum inst { T_ACTIVATE, T_DEACTIVATE, T_ADVANCE, T_COLLECT, T_READ, SEND, RECEIVE };
	public:
		ArbolSintactico * declaraciones;
		inst instruccion;
		intr_robot(ArbolSintactico * l, int m) : declaraciones(l), instruccion(static_cast<inst>(m)) {}
		intr_robot(int m) : instruccion(static_cast<inst>(m)) {}
		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			switch(instruccion){
				case T_ACTIVATE:
					cout << "ACTIVATE:" << endl;
					break;
				case T_DEACTIVATE:
					cout << "DEACTIVATE:" << endl;
					break;
				case T_ADVANCE:
					cout << "ADVANCE:" << endl;
					break;
				case T_COLLECT:
					cout << "COLLECT:" << endl;
					break;
				case T_READ:
					cout << "READ:" << endl;
					break;
				case SEND:
					cout << "SEND:" << endl;
					break;
				case RECEIVE:
					cout << "RECEIVE:" << endl;
					break;
			}
			if (declaraciones != NULL){
				declaraciones -> imprimir(i + 1);
			}
		}
};

class expr_aritmetica : public ArbolSintactico {
		enum inst { SUMA, RESTA, MULT, DIV, MOD, PARENTESIS, NEGATIVO};
	public:
		ArbolSintactico * numero_izq;
		ArbolSintactico * numero_der;
		inst instruccion;
		expr_aritmetica(ArbolSintactico * i,ArbolSintactico * d, int m) : numero_izq(i), numero_der(d), instruccion(static_cast<inst>(m)) {}
		expr_aritmetica(ArbolSintactico * d, int m) : numero_der(d), instruccion(static_cast<inst>(m)) {}
		virtual void imprimir(int i){
			if (numero_izq != NULL){
				numero_izq -> imprimir(i + 1);
			}
			for (int j = 0; j < i; j++) cout << "	";
			switch(instruccion){
				case SUMA:
					cout << "SUMA:" << endl;
					break;
				case RESTA:
					cout << "RESTA:" << endl;
					break;
				case MULT:
					cout << "MULT:" << endl;
					break;
				case DIV:
					cout << "DIV:" << endl;
					break;
				case MOD:
					cout << "MOD:" << endl;
					break;
				case PARENTESIS:
					cout << "PARENTESIS:" << endl;
					break;
				case NEGATIVO:
					cout << "NEGATIVO: " << endl;
			}
			if (numero_der != NULL){
				numero_der -> imprimir(i + 1);
			}
		}
};

class expr_booleana : public ArbolSintactico {
		enum inst { IGUAL, MENOR, MAYOR, MENORIGUAL, MAYORIGUAL, PARENTESIS, DISYUNCION, CONJUNCION, NEGACION};
	public:
		ArbolSintactico * bool_izq;
		ArbolSintactico * bool_der;
		inst instruccion;
		expr_booleana(ArbolSintactico * i,ArbolSintactico * d, int m) : bool_izq(i), bool_der(d), instruccion(static_cast<inst>(m)) {}
		expr_booleana(ArbolSintactico * d, int m) : bool_der(d), instruccion(static_cast<inst>(m)) {}
		virtual void imprimir(int i){
			if (bool_izq != NULL){
				bool_izq -> imprimir(i + 1);
			}
			for (int j = 0; j < i; j++) cout << "	";
			switch(instruccion){
				case IGUAL:
					cout << "IGUAL:" << endl;
					break;
				case MENOR:
					cout << "MENOR:" << endl;
					break;
				case MAYOR:
					cout << "MAYOR:" << endl;
					break;
				case MENORIGUAL:
					cout << "MENORIGUAL:" << endl;
					break;
				case MAYORIGUAL:
					cout << "MAYORIGUAL:" << endl;
					break;
				case PARENTESIS:
					cout << "PARENTESIS:" << endl;
					break;
				case DISYUNCION:
					cout << "DISYUNCION:" << endl;
					break;
				case CONJUNCION:
					cout << "CONJUNCION:" << endl;
					break;
				case NEGACION:
					cout << "NEGACION:" << endl;
					break;

			}
			if (bool_der != NULL){
				bool_der -> imprimir(i + 1);
			}
		}
};

class numero : public ArbolSintactico {
	public:
		int valor;
		numero(int v) : valor(v) {}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "numero: " << valor << endl;
		}
};

class booleano : public ArbolSintactico {
	public:
		bool valor;
		booleano(bool v) : valor(v) {}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "booleano: " << valor << endl;
		}
};


class character : public ArbolSintactico {
	public:
		char valor;
		character(char v) : valor(v) {}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "character: " << valor << endl;
		}
};


class identificador : public ArbolSintactico {
	public:
		string valor;
		identificador(string v) : valor(v) {}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "identificador: " << valor << endl;
		}
};

class me : public ArbolSintactico {
	public:
		me() {}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "ME" << endl;
		}
}; 