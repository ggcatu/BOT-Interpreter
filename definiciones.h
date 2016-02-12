/*
Traductores e Interpretadores (CI-3725)

Autores:
Guillermo Betancourt, carnet 11-10103
Gabriel Giménez, carnet 12-11006

definiciones.h:
Contiene un mapeo palabra -> integer para identificar los tipos de tokens, así como las firmas 
de las clases y funciones implementadas en definiciones.cpp
*/

#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h> 
#include <cstring>
#include <vector>

using namespace std;

// // Caracteres especiales
// #define SPACE 1
// #define TAB 2
// #define ENTER 3

// // Tipos
// #define CHARACTER 4
// #define TRUE 5
// #define FALSE 6
// #define INTEGER 7

// // Variables
// #define IDENTIFIER 8

// // Simbolos
// #define COMA 9
// #define PUNTO 10
// #define DOSPUNTOS 11
// #define PARABRE 12
// #define PARCIERRA 13
// #define SUMA 14
// #define RESTA 15
// #define MULT 16
// #define DIV 17
// #define MOD 18
// #define CONJUNCION 19
// #define DISYUNCION 20
// #define NEGACION 21
// #define MENOR 22
// #define MENORIGUAL 23
// #define MAYOR 24
// #define MAYORIGUAL 25
// #define IGUAL 26

// // Reservadas
// #define CREATE 27
// #define WHILE 28
// #define BOOL 29
// #define IF 30
// #define ELSE 31
// #define INT 32
// #define CHAR 33
// #define ON 34
// #define END 35
// #define EXECUTE 36
// #define ACTIVATION 37
// #define DEACTIVATION 38
// #define DEFAULT 39
// #define BOT 40
// #define ACTIVATE 41
// #define DEACTIVATE 42
// #define ADVANCE 43

// // Entrada y salida
// #define READ 44
// #define SEND 45
// #define RECEIVE 46 
// #define STORE 47

// // Movimientos
// #define COLLECT 48
// #define DROP 49
// #define LEFT 50
// #define RIGHT 51
// #define UP 52
// #define DOWN 53

// // Especial
#define ME 54

// // Comentarios
#define COMENTARIO 55

// // Error
// #define ERROR 56

extern string nToWord[];
string IntToString ( int number );

struct Token{
	int identificador,fila,columna;
	Token (int x, int y, int z);
	virtual string to_string();
};

struct TokenIdentificador:Token {
	string valor;
	TokenIdentificador (int x, int y, int z, string v);
	string to_string();
};

struct TokenInteger:Token {
	int valor;
	TokenInteger (int x, int y, int z, int v);
	string to_string();
};

struct TokenCharacter:Token {
	char valor;
	TokenCharacter (int x, int y, int z, char v);
	string to_string();
};

struct TokenError:Token{
	string valor;
	TokenError(int x, int y, int z, string v);
	string to_string();
};

int lastOcurrence(char* s, char x);
int numOcurrence(char* s, char x, int start);
void print_vector(vector<Token *> vector);
void print_errors(vector<Token *> vector);

#endif