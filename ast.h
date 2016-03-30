/*
Traductores e Interpretadores (CI-3725)

Autores:
Guillermo Betancourt, carnet 11-10103
Gabriel Giménez, carnet 12-11006

ast.h:
Contiene la definicion del arbol sintactico asi como la de la tabla de simbolos utilizadas
en el proyecto
*/

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <ctype.h>
#include <cstdlib>
#include "arb_def.h"
#include "robot.h"
using namespace std;
#define NUMEROS 1
#define BOOLEANOS 2
#define CHARACTERS 3
#define CONDICION 4

/* Definiciones externas (parser.y)
	que permiten compartir el codigo.
*/

extern tabla_simbolos * head_table;
extern int yylineno;
extern char error_strp[1000];
extern map<string,Robot *> robots;
extern int yyparse();
extern map<int, map<int, variable *> > matriz_bot;
extern Robot * working_bot;
/* Definicion de la clase numero */
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
		virtual int * get_value(){
			return &valor;
		}
};

/* Definicion de la clase booleano */
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
		virtual bool * get_bool(){
			return &valor;
		}
};

/* Definicion de la clase caracter */
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

		virtual char * get_character(){
			return &valor;
		}
};



/* Definicion de la clase raiz */

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
		virtual void ejecutar(){
			//cout << "Ejecutando raiz" << endl;
			head_table = new tabla_simbolos(&table);
			//head_table->print_rob();
			ejecucion -> ejecutar();
			if (head_table->padre != NULL){
				head_table = head_table->padre;
			}
		}
};

/* Definicion de la clase instruccion
	es utilizada para generar una lista de instrucciones */

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

		virtual void ejecutar(){
			if (left != NULL){
				left->ejecutar();
			}
			rigth->ejecutar();
		}

		virtual void activate(){
			if (left != NULL){
				left->activate();
			}
			rigth->activate();
		}

		virtual void deactivate(){
			if (left != NULL){
				left->deactivate();
			}
			rigth->deactivate();
		}

		virtual bool advance(){
			if (left != NULL){
				if (left->advance()){
					return true;
				}
			}
			if (rigth->advance()){
				return true;
			}
			return false;
		}

		virtual void add_comportamiento(ArbolSintactico * comp){
			if (left != NULL){
				left->add_comportamiento(comp);
			}
			rigth->add_comportamiento(comp);
		}

		virtual void add_value(variable * value){
			if (left != NULL){
				left->add_value(value);
			}
			rigth->add_value(value);
		}
};

/* Definicion de la clase nodo de movimientos */

class intr_movimiento : public ArbolSintactico {
		enum mov { UP, DOWN, LEFT, RIGTH };
	public:
		ArbolSintactico * down;
		mov movimiento;
		intr_movimiento(ArbolSintactico * l, int m) : down(l), movimiento(static_cast<mov>(m)) {check();}
		intr_movimiento(int m) : movimiento(static_cast<mov>(m)) {}
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
			if (down != NULL){
				down -> imprimir(i + 1);
			}
		}
		/* Chequea que la expresion recibida sea un entero
			Se debera luego chequear que sea mayor a 0 */
		virtual void check(){
			if (down->ident != NUMEROS){
				sprintf(error_strp,"Error de tipo, se esperaba un booleano [LINEA: %d]", yylineno);
				throw error_strp;
			}
		}

		virtual void ejecutar(){
			int mov = 1;
			if (down != NULL){
				mov = * down->get_value();
				if (mov < 0){
					sprintf(error_strp,"Error se esta moviendo en la matriz con un numero negativo. [LINEA: %d]", yylineno);
					throw error_strp;
				}
			}
			switch(movimiento){
				case UP:

					working_bot->posicion[0] += mov;
					break;
				case DOWN:
					working_bot->posicion[0] -= mov;
					break;
				case LEFT:
					working_bot->posicion[1] -= mov;
					break;
				case RIGTH:
					working_bot->posicion[1] += mov;
					break;
			}
		//	cout << "Posicion nueva:" << working_bot->posicion[0] << ", " << working_bot->posicion[1] << " ADDED : " << mov << endl;
		}
};



/* Definicion de la clase guardia, contiene instrucciones
	que requieren la evaluacion de una condicion */

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
		/* Se verifica que la condicion sea del tipo BOOLEANA */
		virtual void check(){
			if(condicion->ident != BOOLEANOS){
				sprintf(error_strp,"Error de tipo, se esperaba un booleano [LINEA: %d]", yylineno);
				throw error_strp;
			}
		}

		virtual void ejecutar(){
			switch(instruccion){
				case IF:
					if (* condicion->get_bool()) {
						cuerpo -> ejecutar();
					}
					break;
				case IF_ELSE:
					if (* condicion->get_bool()) {
						cuerpo -> ejecutar();
					} else {
						cuerpo_else -> ejecutar();
					}		
					break;
				case WHILE:
					while (* condicion->get_bool()) {
						cuerpo -> ejecutar();
					}
					break;
			}
			
		}
};


/* Definicion de la clase que engloba las expresiones aritmeticas y devuelven un numero */
class expr_aritmetica : public ArbolSintactico {
		enum inst { SUMA, RESTA, MULT, DIV, MOD, PARENTESIS, NEGATIVO, NUMERO};
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
					break;
				case NUMERO:
					cout << "NUMERO: " << endl;
					break;
			}
			if (numero_der != NULL){
				numero_der -> imprimir(i + 1);
			}
		}
		/* Se chequea que este bien compuesta la expresion, chequeando si son numeros */
		virtual void check(){
			if (instruccion == PARENTESIS){
				ident = numero_der->ident;
			}
			if (instruccion != NEGATIVO && instruccion != NUMERO && instruccion != PARENTESIS){
				if (numero_der->ident != NUMEROS ||
					numero_izq->ident != NUMEROS){
					sprintf(error_strp,"Error de tipo, se esperaban dos int [LINEA: %d]", yylineno);
					throw error_strp;
				}
			} else {
				if (numero_der->ident != NUMEROS && instruccion != PARENTESIS){
					sprintf(error_strp,"Error de tipo, se esperaba un int [LINEA: %d]", yylineno);
					throw error_strp;
				}
			}
		}

		virtual int * get_value(){
			switch(instruccion){
				case SUMA:
					return new int(*numero_der->get_value() + *numero_izq->get_value());
				case RESTA:
					return new int(*numero_izq->get_value() - *numero_der->get_value());
				case MULT:
					return new int(*numero_der->get_value() * *numero_izq->get_value());
				case DIV:
					if(*numero_der->get_value() == 0) { 
						sprintf(error_strp,"Error division por cero. [LINEA: %d]", yylineno);
						throw error_strp;
						}
					return new int(*numero_izq->get_value() / *numero_der->get_value());
				case MOD:
					return new int(*numero_der->get_value() % *numero_izq->get_value());
				case PARENTESIS:
					return numero_der->get_value();
				case NEGATIVO:
					return new int(-*numero_der->get_value());
				case NUMERO:
					return numero_der->get_value();
			}
		}

		virtual bool * get_bool(){
			return numero_der->get_bool();
		}
};

/* Definicion de la clase que engloba instrucciones que 
	calificamos como "extra" */

class intr_extra : public ArbolSintactico {
		enum mov { STORE, DROP };
	public:
		ArbolSintactico * down;
		mov movimiento;
		intr_extra(ArbolSintactico * l, int m) : down(l), movimiento(static_cast<mov>(m)) {}
		virtual void imprimir(int i){
			for (int j = 0; j < i; j++) cout << "	";
			switch(movimiento) {	
				case STORE:
					cout << "STORE:" << endl;
					break;
				case DROP:
					cout << "DROP:" << endl;
					break;
			}
			down -> imprimir(i + 1);
		}
		virtual void ejecutar(){
			switch(movimiento){
					case STORE:
						switch(head_table->valores["me"]->tipo){
							case NUMEROS: {
								if (down -> ident != NUMEROS){
									sprintf(error_strp,"Error de tipo al hacer STORE, se esperaba un int [LINEA: %d]", yylineno);
									throw error_strp;
								} else {
									head_table->valores["me"]->init = true; 
									static_cast<variable_int * >(head_table->valores["me"])->valor = static_cast<expr_aritmetica *>(down)->get_value(); 
								}
								break;
							}
							case CHARACTERS: {
								if (down -> ident != CHARACTERS){
									sprintf(error_strp,"Error de tipo al hacer STORE, se esperaba un character [LINEA: %d]", yylineno);
									throw error_strp;
								} else {
									head_table->valores["me"]->init = true; 
									static_cast<variable_char * >(head_table->valores["me"])->valor = static_cast<character *>(down)->get_character(); 
								}
								break;
							}
							case BOOLEANOS: {
								if (down -> ident != BOOLEANOS){
									sprintf(error_strp,"Error de tipo al hacer STORE, se esperaba un booleano [LINEA: %d]", yylineno);
									throw error_strp;
								} else {
									head_table->valores["me"]->init = true; 
									static_cast<variable_bool * >(head_table->valores["me"])->valor = static_cast<booleano *>(down)->get_bool(); 
								}
								break;
							}
						}
					break;
					case DROP:
						//cout <<"DROPEO" << endl;
						switch(down -> ident){
							case NUMEROS: {

								variable_int * tmp = new variable_int(NUMEROS, down->get_value(), true);
								matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]] = tmp;
								break;
							}
							case CHARACTERS: {
								variable_char * tmp = new variable_char(CHARACTERS, down->get_character(), true);
								matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]] = tmp;
								break;
							}
							case BOOLEANOS: {
								variable_bool * tmp = new variable_bool(BOOLEANOS, down->get_bool(), true);
								matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]] = tmp;
								break;
							}
						}
					break;
			}
			
		}
};

/* Definicion de la clase que engloba las expresiones booleanas y devuelven un booleano */
class expr_booleana : public ArbolSintactico {
		enum inst { IGUAL, MENOR, MAYOR, MENORIGUAL, MAYORIGUAL, PARENTESIS, DISYUNCION, CONJUNCION, NEGACION, CONSTANTE};
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
				case CONSTANTE:
					cout << "COSTANTE:" << endl;
					break;
			}
			if (bool_der != NULL){
				bool_der -> imprimir(i + 1);
			}
		}
		/* Se chequea que este bien compuesta la expresion. */
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

		virtual bool * get_bool(){
			switch(instruccion){
				case IGUAL:
					return new bool(*bool_izq->get_value() == *bool_der->get_value());
				case MENOR:
					return new bool(*bool_izq->get_value() < *bool_der->get_value());
				case MAYOR:
					// cout << *bool_izq->get_value() << " > " << *bool_der->get_value() << endl;
					return new bool(*bool_izq->get_value() > *bool_der->get_value());
				case MENORIGUAL:
					return new bool(*bool_izq->get_value() <= *bool_der->get_value());
				case MAYORIGUAL:
					return new bool(*bool_izq->get_value() >= *bool_der->get_value());
				case PARENTESIS:
					return bool_der->get_bool();
				case DISYUNCION:
					return new bool(*bool_der->get_bool() || *bool_izq->get_bool() );
				case CONJUNCION:
					return new bool(*bool_der->get_bool() && *bool_izq->get_bool() );
				case NEGACION:
					return new bool(!*bool_der->get_bool());
				case CONSTANTE:
					return bool_der->get_bool();
			}
		}
};

/* Definicion de la clase que engloba las condiciones de los comportamientos del robot*/
class on_condicion : public ArbolSintactico {
	public:
		enum inst { ACTIVATION, DEACTIVATION, DEFAULT, EXPR};
		inst condicion;
		ArbolSintactico * expr;
		on_condicion(int v) :  ArbolSintactico(CONDICION), condicion(static_cast<inst>(v)){ add_variable(0,0); }
		on_condicion(int v, ArbolSintactico * l) : expr(l), ArbolSintactico(CONDICION), condicion(static_cast<inst>(v)){check();}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			switch(condicion){
				case ACTIVATION:
				cout << "ACTIVATION: " << endl;
				break;
				case DEACTIVATION:
				cout << "DEACTIVATION: " << endl;
				break;
				case DEFAULT:
				cout << "DEFAULT: " << endl;
				break;
				case EXPR:
				cout << "EXPRESION: " << endl;
				expr->imprimir(i);
				break;
			}		
		}
		/* Permite llevar un control del numero de Activation, Deactivation, Default declarados.*/
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
			if (condicion == EXPR){
				check();
			}
			head_table->mapa[nombre] = 0;	
		}

		/* Verifica que el ultimo comportamiento sea el default */
		virtual void check(){
			if (head_table->mapa.count("default") > 0) {
					sprintf(error_strp,"default debe ser el ultimo comportamiento. [LINEA: %d]", yylineno);
					throw error_strp;
			}
		}

		virtual bool * get_bool(){
			return expr->get_bool();
		}


};

/* Definicion de la clase identificador */
class identificador : public ArbolSintactico {
	public:
		string valor;
		tabla_simbolos * tabla;
		/* Chequea si es una variable solo local o tiene mayor alcance */
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
				case NUMEROS:
					ty = "numero";
				break;
				case BOOLEANOS:
					ty = "booleano";
				break;
				case CHARACTERS:
					ty = "character";
				break;
			}
			for (int j = 0; j < i; j++) cout << "	";
			cout << "identificador: " << valor.c_str() << " tipo: " << ty.c_str() << endl;
		}

		/* Se verifica que la variabla este declarada en el contexto y su alcance */
		void check() {
			if ( ident == 0 ) {
				const char * c = valor.c_str();
				sprintf(error_strp,"%s no ha sido declarado [LINEA: %d]", c, yylineno);
				throw error_strp;}
		}
		/* Se agrega la variable a las tablas de simbolos correspondientes */
		void add_variable(int tipo, bool doble){
			variable * temp;
				switch(tipo){
					case NUMEROS:
						temp = new variable_int(tipo);
					break;
					case BOOLEANOS:
						temp = new variable_bool(tipo);
					break;
					case CHARACTERS:
						temp = new variable_char(tipo);
					break;
			}
			if (doble){
				if (head_table->padre->mapa.count(valor) > 0) {
					const char * c = valor.c_str();
					sprintf(error_strp,"%s ya habia sido declarada antes. [LINEA: %d]", c, yylineno);
					throw error_strp;
				}
				(head_table->padre->robots)[valor] = new Robot(tipo, head_table);
				// Declaracion de simbolos
				head_table->padre->mapa[valor] = tipo;
				head_table->mapa["me"] = tipo;
				// Declaracion de valores
				
				head_table->valores["me"] = temp;
				head_table->padre->valores[valor] = head_table->valores["me"];
			} else {
				// if (head_table->mapa.count(valor) > 0) {
				// 	const char * c = valor.c_str();
				// 	sprintf(error_strp,"%s ya habia sido declarada antes. [LINEA: %d]", c, yylineno);
				// 	throw error_strp;
				// }
				head_table->valores[valor] = temp;
				head_table->mapa[valor] = tipo;
			}
			ident = tipo;
			tabla = head_table;
		}

		virtual int * get_value(){
			if (working_bot != NULL){
				return static_cast<variable_int * >(head_table->valores[valor])->valor;
			}
			 // cout << valor << head_table->valores[valor]->init << " ACTIVATED " << endl;
			 // cout << (head_table->padre->robots)[valor]->activated << " ACTIVATED " << endl;
//			cout << "trying " << valor << (head_table->padre->robots)[valor]->activated <<  endl;
			if (head_table->valores[valor]->init && (head_table->robots)[valor]->activated){
				return static_cast<variable_int * >(head_table->valores[valor])->valor;
			} else {
				sprintf(error_strp,"Error se esta utilizando la variable sin inicializar o activar el robot");
				throw error_strp;
			}
			
		}

		virtual bool * get_bool(){
			if (head_table->valores[valor]->init && (head_table->padre->robots)[valor]->activated){
				return static_cast<variable_bool * >(head_table->valores[valor])->valor;
			} else {
				sprintf(error_strp,"Error se esta utilizando la variable sin inicializar o activar el robot");
				throw error_strp;
			}
		}

		virtual char * get_character(){
			if (head_table->valores[valor]->init && (head_table->padre->robots)[valor]->activated){
				return static_cast<variable_char * >(head_table->valores[valor])->valor;
			} else {
				sprintf(error_strp,"Error se esta utilizando la variable sin inicializar o activar el robot");
				throw error_strp;
			}
		}

		virtual void activate(){
			(head_table->robots)[valor]->activate();
		}

		virtual void deactivate(){
			(head_table->robots)[valor]->deactivate();
		}

		virtual bool advance(){
			(head_table->robots)[valor]->advance();
			return false;
		}

		virtual void add_comportamiento(ArbolSintactico * comp){
			(head_table->padre->robots)[valor]->comportamientos = comp;
		}

		virtual void add_value(variable * value){
			head_table->valores[valor]->init = true;
			head_table->valores[valor] = matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]]->clone();
		}
};


/* Definicion de la clase que engloba las instrucciones del robot */

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

		virtual void ejecutar(){
			switch(instruccion){
				case T_ACTIVATE:
					declaraciones->activate();
					break;
				case T_DEACTIVATE:
					declaraciones->deactivate();
					break;
				case T_ADVANCE:
					declaraciones->advance();
					break;
				case SEND:
					if ( head_table->valores["me"]->init ) {
						switch(head_table->valores["me"]->tipo){
							case NUMEROS: {
								int * numero = static_cast<variable_int * >(head_table->valores["me"])->valor;
								cout << *numero;
								break;
							}
							case CHARACTERS: {
								char * character = static_cast<variable_char * >(head_table->valores["me"])->valor;
								cout << *character;
								break;
							}
							case BOOLEANOS: {
								bool * valor = static_cast<variable_bool * >(head_table->valores["me"])->valor;
								cout << * valor;
								break;
							}
						}
					} else {
						sprintf(error_strp,"Error se esta utilizando la variable sin inicializar el bot. [LINEA: %d]", yylineno);
						throw error_strp;
					}
				break;

				case T_READ:{
					// cout << " Donde estamos " << endl;
					// head_table->print_val();
					switch(head_table->valores["me"]->tipo){
						case NUMEROS: {
							int leidos, valor;
							char numero[11];
							leidos = scanf("%s", numero);
							string tmp(numero);
							for(int i = 0; i < tmp.length(); i++){
								if (!isdigit(numero[i])){
									sprintf(error_strp,"Error de lectura en el terminal, se esperaba un entero.", yylineno);
									throw error_strp;
								}
							}
							valor = atoi(numero);						
							head_table->valores["me"]->init = true; 
							static_cast<variable_int * >(head_table->valores["me"])->valor = new int(valor);
							break;
							}
						case CHARACTERS: {
							char valor;
							bool assigned = false;
							char numero[11];
							scanf("%s", numero);
							string tmp(numero);
							if (!tmp.compare("\\n") && !assigned){
								assigned = true;
								valor = '\n';
							}
							if (!tmp.compare("\\t") && !assigned){
								assigned = true;
								valor = '\t';
							}
							if (!tmp.compare("\\'") && !assigned){
								assigned = true;
								valor = '\'';
							}
							if (!assigned){
								if(tmp.length() > 1){
									sprintf(error_strp,"Error de lectura en el terminal, se esperaba un caracter");
									throw error_strp;
								}
								valor = tmp[0];
							}
							head_table->valores["me"]->init = true; 
							static_cast<variable_char * >(head_table->valores["me"])->valor = new char(valor);
							break;
						}
						case BOOLEANOS: {
							char valor[100];
							scanf("%s", valor);
							string temp(valor);
							if (!(temp.compare("true"))){
								head_table->valores["me"]->init = true; 
								static_cast<variable_bool * >(head_table->valores["me"])->valor = new bool(true);
							}
							if (!(temp.compare("false"))){
								head_table->valores["me"]->init = true; 
								static_cast<variable_bool * >(head_table->valores["me"])->valor = new bool(false);
							}

							if (temp.compare("true") && temp.compare("false")){
								sprintf(error_strp,"Error de lectura en el terminal, se esperaba un booleano");
								throw error_strp;
							}
						}
					}
				}
				break;
				case T_COLLECT:
				variable * elem = matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]];
				if (elem == NULL){
						sprintf(error_strp,"Error, recogiendo un valor vacío en la matriz [LINEA: %d]", yylineno);
						throw error_strp;
				}
				if (declaraciones == NULL){
					if (matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]]->tipo != head_table->valores["me"]->tipo){
						sprintf(error_strp,"Este robot no puede recoger este valor, no coinciden los tipos [LINEA: %d]", yylineno);
						throw error_strp;
					}
					// Por que clonar no sirve?
					switch(head_table->valores["me"]->tipo){
						case NUMEROS: {
							int valor = * static_cast<variable_int *>(matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]])->valor;
							static_cast<variable_int * >(head_table->valores["me"])->valor = new int(valor);
							break;
						}
						case BOOLEANOS: {
							bool valor = * static_cast<variable_bool *>(matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]])->valor;
							static_cast<variable_bool * >(head_table->valores["me"])->valor = new bool(valor);
							break;
						}
						case CHARACTERS: {
							char valor = * static_cast<variable_char *>(matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]])->valor;
							static_cast<variable_char * >(head_table->valores["me"])->valor = new char(valor);
							break;
						}
					}
					head_table->valores["me"]->init = true;
					// head_table->valores["me"] =->clone();
				} else {
					if (elem->tipo != head_table->mapa["me"]){
						sprintf(error_strp,"Este robot no puede recoger este valor, no coinciden los tipos [LINEA: %d]", yylineno);
						throw error_strp;
					}
					declaraciones->add_value(matriz_bot[working_bot->posicion[0]][working_bot->posicion[1]]);
				}			
				break;
			}
		}
};

/* Definicion de la clase me */
class me : public ArbolSintactico {
	public:
		me() {ident = head_table->mapa.at("me");}
		virtual void imprimir(int i) {
			for (int j = 0; j < i; j++) cout << "	";
			cout << "ME" << endl;
		}

		virtual int * get_value(){
			if (head_table->valores["me"]->init && working_bot->activated){
				return static_cast<variable_int * >(head_table->valores["me"])->valor;
			} else {
				sprintf(error_strp,"Error se esta utilizando la variable ME sin inicializar o activar el robot");
				throw error_strp;
			}
			
		}

		virtual bool * get_bool(){
					if (head_table->valores["me"]->init && working_bot->activated){
				return static_cast<variable_bool * >(head_table->valores["me"])->valor;
			} else {
				sprintf(error_strp,"Error se esta utilizando la variable ME sin inicializar o activar el robot");
				throw error_strp;
			}
		}

		virtual char * get_character(){
			if (head_table->valores["me"]->init && working_bot->activated){
				return static_cast<variable_char * >(head_table->valores["me"])->valor;
			} else {
				sprintf(error_strp,"Error se esta utilizando la variable ME sin inicializar o activar el robot");
				throw error_strp;
			}
		}
}; 

/* Definicion de la clase declaracion */
class declaracion : public ArbolSintactico {
	public:
		ArbolSintactico * tipo;
		ArbolSintactico * variable;
		ArbolSintactico * comportamiento;
		declaracion(ArbolSintactico * t, ArbolSintactico * i) : tipo(t), variable(i) {
			variable->add_variable(tipo->ident,1);
			
		}
		virtual void imprimir(int i){
			tipo -> imprimir(i);	
			variable -> imprimir(i);
			if (comportamiento != NULL ) {
				comportamiento -> imprimir(i+1);	
			}
		}

		virtual void add_comportamiento(ArbolSintactico * comp){
			comportamiento = comp;
			variable->add_comportamiento(comp);
		}
};

/* Definicion de la clase que engloba los comportamientos de los bots */
class inside_bot : public ArbolSintactico {
	
	public:
		enum inst { ACTIVATION, DEACTIVATION, DEFAULT, EXPR};
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
			/* Chequea que la condicion sea valida, o expr, o default o activation o deactivation*/
			if (condicion->ident != BOOLEANOS && condicion->ident != CONDICION){
				sprintf(error_strp,"Error de tipo, la condicion debe ser booleana [LINEA: %d]", yylineno);
				throw error_strp;
			}
		}

		virtual void ejecutar(){
			instruccion->ejecutar();
		}

		virtual void activate(){
			if (static_cast<on_condicion *>(condicion)->condicion == 0){
				instruccion->ejecutar();
 			}
		}

		virtual void deactivate(){
			if (static_cast<on_condicion *>(condicion)->condicion == 1){
				instruccion->ejecutar();
			}
		}

		virtual bool advance(){
			if (static_cast<on_condicion *>(condicion)->condicion == 2 || 
				static_cast<on_condicion *>(condicion)->condicion == 3 && * condicion->get_bool()){ 
					instruccion -> ejecutar();
					return true;
			}
			return false;
		}
};
