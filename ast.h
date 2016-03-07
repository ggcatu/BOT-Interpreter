#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <map>
using namespace std;
#define NUMEROS 1
#define BOOLEANOS 2
#define CHARACTERS 3
#define CONDICION 4
class tabla_simbolos {
	public:
		map<string,int> mapa;
		tabla_simbolos * padre;
		tabla_simbolos(){padre = NULL;};
		void print() { 
			for(map<string, int>::const_iterator it = mapa.begin();
			    it != mapa.end(); ++it)
			{
			    cout << it->first.c_str() << " " << it->second << endl;
			}
		}
};

extern tabla_simbolos * head_table;
extern int yylineno;
extern char error_strp[1000];

class ArbolSintactico {
	ArbolSintactico * first;
	public:
		int ident;
		int linea;
		bool is_type;
		ArbolSintactico(): is_type(0){};
		ArbolSintactico(int i): ident(i),is_type(0) {};
		ArbolSintactico(ArbolSintactico * l): first(l),is_type(0) {};
		virtual void imprimir(int i){ if(first != NULL) first->imprimir(i); }; 
		virtual int get_ident(){ return ident; }
		virtual void add_variable(int tipo, bool doble){ return; }
		virtual void check(){;}
};

class raiz : public ArbolSintactico {
	public:
		ArbolSintactico * declaraciones;
		ArbolSintactico * ejecucion;
		tabla_simbolos table;
		raiz(ArbolSintactico * l) : ejecucion(l), table() {}
		raiz(ArbolSintactico * d, ArbolSintactico * l) : declaraciones(d), ejecucion(l), table(){}
		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			for (int j = 0; j < 15; j++) cout << "-";
			cout << endl;
			for (int j = 0; j < i; j++) cout << "	";
			if(declaraciones != NULL){
				cout << "DECLARACIONES:" << endl;
				declaraciones -> imprimir(i+1);
			}
			for (int j = 0; j < i; j++) cout << "	";
			if(ejecucion != NULL){
				cout << "SECUENCIACION:" << endl;
				ejecucion -> imprimir(i+1);
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

		virtual void add_variable(int tipo, bool doble) {
			if (rigth != NULL){
				rigth->add_variable(tipo, doble);
			}
			if (left != NULL){
				left->add_variable(tipo, doble);
			}
		}

		virtual void check(){
			rigth->check();
			if (left != NULL){
				left->check();
			}
		}
};


class intr_movimiento : public ArbolSintactico {
		enum mov { UP, DOWN, LEFT, RIGTH };
	public:
		ArbolSintactico * down;
		mov movimiento;
		intr_movimiento(ArbolSintactico * l, int m) : down(l), movimiento(static_cast<mov>(m)) {check();}
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
			down -> imprimir(i + 1);
		}
		virtual void check(){
			if (down->ident != NUMEROS){
				sprintf(error_strp,"Error de tipo, se esperaba un booleano [LINEA: %d]", yylineno);
				throw error_strp;
			}
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
			down -> imprimir(i + 1);
		}
};

class intr_guardia : public ArbolSintactico {
		enum inst { IF, IF_ELSE, WHILE };
	public:
		ArbolSintactico * condicion;
		ArbolSintactico * cuerpo;
		ArbolSintactico * cuerpo_else;
		inst instruccion;
		intr_guardia(ArbolSintactico * l, ArbolSintactico * d, int m) : condicion(l), cuerpo(d), instruccion(static_cast<inst>(m)) {check();}
		intr_guardia(ArbolSintactico * l, ArbolSintactico * d, ArbolSintactico * ce, int m) : 
													   condicion(l), cuerpo(d), cuerpo_else(ce), instruccion(static_cast<inst>(m)) {check();}

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
				condicion -> imprimir(i + 1);
			for (int j = 0; j < i; j++) cout << "	";
			cout << "CUERPO: " << endl;
				cuerpo -> imprimir(i + 1);
			if (cuerpo_else != NULL){
				for (int j = 0; j < i; j++) cout << "	";
					cout << "ELSE: " << endl;
					cuerpo_else -> imprimir(i + 1);
			}
		}

		virtual void check(){
			if(condicion->ident != BOOLEANOS){
				sprintf(error_strp,"Error de tipo, se esperaba un booleano [LINEA: %d]", yylineno);
				throw error_strp;
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
					cout << "SEND." << endl;
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
		expr_aritmetica(ArbolSintactico * i,ArbolSintactico * d, int m) : numero_izq(i), numero_der(d), instruccion(static_cast<inst>(m)), ArbolSintactico(NUMEROS) {check();}
		expr_aritmetica(ArbolSintactico * d, int m) : numero_der(d), instruccion(static_cast<inst>(m)), ArbolSintactico(NUMEROS) {check();}
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
		virtual void check(){
			if (instruccion != NEGATIVO){
				if (numero_der->ident != NUMEROS ||
					numero_izq->ident != NUMEROS){
					sprintf(error_strp,"Error de tipo, se esperaban dos int [LINEA: %d]", yylineno);
						throw error_strp;
				}
			} else {
				if (numero_der->ident != NUMEROS){
					sprintf(error_strp,"Error de tipo, se esperaba un int [LINEA: %d]", yylineno);
					throw error_strp;
				}
			}
		}
};

class expr_booleana : public ArbolSintactico {
		enum inst { IGUAL, MENOR, MAYOR, MENORIGUAL, MAYORIGUAL, PARENTESIS, DISYUNCION, CONJUNCION, NEGACION};
	public:
		ArbolSintactico * bool_izq;
		ArbolSintactico * bool_der;
		inst instruccion;
		expr_booleana(ArbolSintactico * i,ArbolSintactico * d, int m) : bool_izq(i), bool_der(d), instruccion(static_cast<inst>(m)), ArbolSintactico(BOOLEANOS) {check();}
		expr_booleana(ArbolSintactico * d, int m) : bool_der(d), instruccion(static_cast<inst>(m)), ArbolSintactico(BOOLEANOS) {check();}
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
		virtual void check(){
				switch(instruccion){
				case IGUAL:
					if (bool_der->ident != bool_izq->ident){
						sprintf(error_strp,"Error de tipo, los tipos no son iguales [LINEA: %d]", yylineno);
						throw error_strp;
					}
					break;
				case MENOR:
				case MAYOR:
				case MENORIGUAL:
				case MAYORIGUAL:
					if (bool_der->ident != NUMEROS ||
						bool_izq->ident != NUMEROS) {
						sprintf(error_strp,"Error de tipo, se esperaban dos int [LINEA: %d]", yylineno);
						throw error_strp;
					}
					break;
				case PARENTESIS:
				case NEGACION:
					if (bool_der->ident != BOOLEANOS){
						sprintf(error_strp,"Error de tipo, se esperaba un booleano [LINEA: %d]", yylineno);
						throw error_strp;
					}
					break;
				case DISYUNCION:
				case CONJUNCION:
					if (bool_der->ident != BOOLEANOS ||
						bool_izq->ident != BOOLEANOS){
						sprintf(error_strp,"Error de tipo, se esperaban dos booleanos [LINEA: %d]", yylineno);
						throw error_strp;
					}
				break;
			}
		}
};


class on_condicion : public ArbolSintactico {
	enum inst { ACTIVATION, DEACTIVATION, DEFAULT };
	public:
		inst condicion;
		on_condicion(int v) :  ArbolSintactico(CONDICION), condicion(static_cast<inst>(v)){ add_variable(0,0); }
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			switch(condicion){
				case ACTIVATION:
				cout << "ON ACTIVATION: " << endl;
				break;
				case DEACTIVATION:
				cout << "ON DEACTIVATION: " << endl;
				break;
				case DEFAULT:
				cout << "ON DEFAULT: " << endl;
				break;
			}		
		}

		virtual void add_variable(int i, int doble) {
			string nombre;
			switch(condicion){
				case ACTIVATION:
				nombre = "activation";
				break;
				case DEACTIVATION:
				nombre = "deactivation";
				break;
				case DEFAULT:
				nombre = "default";
				break;
			}
			if (head_table->mapa.count(nombre) > 0) {
				const char * c = nombre.c_str();
				sprintf(error_strp,"%s ya habia sido declarada antes. [LINEA: %d]", c, yylineno);
				throw error_strp;
			}
			if (condicion == ACTIVATION || condicion == DEACTIVATION){
				if (head_table->mapa.count("default") > 0) {
					sprintf(error_strp,"default debe ser el ultimo comportamiento. [LINEA: %d]", yylineno);
					throw error_strp;
				}
			}
			head_table->mapa[nombre] = 0;	
		}


};

class numero : public ArbolSintactico {
	public:
		int valor;
		numero(): ArbolSintactico(NUMEROS) {is_type = 1;}
		numero(int v) : valor(v), ArbolSintactico(NUMEROS){}
		virtual void imprimir(int i) {
			if (!is_type){
				for (int j = 0; j < i; j++) cout << "	";
				cout << "numero: " << valor << endl;
			}
		}
};

class booleano : public ArbolSintactico {
	public:
		bool valor;
		booleano(): ArbolSintactico(BOOLEANOS) {is_type = 1;}
		booleano(bool v) : valor(v), ArbolSintactico(BOOLEANOS) {}
		virtual void imprimir(int i) {
			if (!is_type){
				for (int j = 0; j < i; j++) cout << "	";
				cout << "booleano: " << valor << endl;
			}
		}
};


class character : public ArbolSintactico {
	public:
		char valor;
		character() : ArbolSintactico(CHARACTERS) {is_type = 1;}
		character(char v) : valor(v), ArbolSintactico(CHARACTERS) {}
		virtual void imprimir(int i) {
			if (!is_type){
				for (int j = 0; j < i; j++) cout << "	";
				cout << "character: " << valor << endl;
			}
		}
};


class identificador : public ArbolSintactico {
	public:
		string valor;
		tabla_simbolos * tabla;
		identificador(string v) : valor(v) {
			if (head_table->mapa.count("me") > 0 ){
				if (head_table->mapa.count(v) > 0){
					ident = head_table->mapa.at(v);
				} else {
					ident = 0;
				}
				tabla = head_table;
			} else {
				tabla_simbolos * rtmp;
				rtmp = head_table;
				while (head_table != NULL){ 
					if (head_table->mapa.count(v) > 0){
						ident = head_table->mapa.at(v);
						tabla = head_table;
						break; 
					} 
					head_table = head_table->padre;
				}
				head_table = rtmp;
			}	
		}
		virtual void imprimir(int i) {
			string ty;
			switch (ident){
				case 1:
					ty = "numero";
				break;
				case 2:
					ty = "booleano";
				break;
				case 3:
					ty = "character";
				break;
			}
			for (int j = 0; j < i; j++) cout << "	";
			cout << "identificador: " << valor.c_str() << " tipo: " << ty.c_str() << endl;
		}
		void check() {
			if ( ident == 0 ) {
				const char * c = valor.c_str();
				sprintf(error_strp,"%s no ha sido declarado [LINEA: %d]", c, yylineno);
				throw error_strp;}
		}
		void add_variable(int tipo, bool doble){
			if (doble){
				if (head_table->padre->mapa.count(valor) > 0) {
					const char * c = valor.c_str();
					sprintf(error_strp,"%s ya habia sido declarada antes. [LINEA: %d]", c, yylineno);
					throw error_strp;
				}
				head_table->padre->mapa[valor] = tipo;
				head_table->mapa["me"] = tipo;
			} else {
				head_table->mapa[valor] = tipo;
			}
			// cout << "--------------" <<endl;
			// head_table->print();
			// cout << "--------------" <<endl;
			// head_table->padre->print();
			ident = tipo;
			tabla = head_table;
		}
};

class me : public ArbolSintactico {
	public:
		me() {ident = head_table->mapa.at("me");}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "ME" << endl;
		}
}; 


class declaracion : public ArbolSintactico {
	public:
		ArbolSintactico * tipo;
		ArbolSintactico * variable;
		ArbolSintactico * comportamiento;
		declaracion(ArbolSintactico * t, ArbolSintactico * i) : tipo(t), variable(i) {variable->add_variable(tipo->ident,1);}
		//declaracion(ArbolSintactico * t, ArbolSintactico * i, ArbolSintactico * c) : tipo(t), variable(i), comportamiento(c) {variable->add_variable(tipo->ident,1);}
		virtual void imprimir(int i){
			tipo -> imprimir(i);	
			variable -> imprimir(i);
			for (int j = 0; j < i; j++) cout << "	";
			cout << "ON: "<< endl;
			if (comportamiento != NULL ) {
				comportamiento -> imprimir(i+1);	
			}
			for (int j = 0; j < i; j++) cout << "	";
			cout << "END" << endl;
		}
};


class inside_bot : public ArbolSintactico {
	public:
		ArbolSintactico * condicion;
		ArbolSintactico * instruccion;
		inside_bot(ArbolSintactico * t, ArbolSintactico * i) : condicion(t), instruccion(i){check();}
		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			cout << "ON" << endl;
			condicion -> imprimir(i);	
			instruccion -> imprimir(i+1);
			for (int j = 0; j < i; j++) cout << "	";
			cout << "END" << endl;
		}
		virtual void check(){
			if (condicion->ident != BOOLEANOS && condicion->ident != CONDICION){
				sprintf(error_strp,"Error de tipo, la condicion debe ser booleana [LINEA: %d]", yylineno);
				throw error_strp;
			}
		}
};
