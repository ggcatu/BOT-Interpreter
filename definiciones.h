#include <string>
using namespace std;
//	Literales
#define SPACE 1
#define TAB 2
//#define ENTER 3

//	Tipos
//#define CHARACTER 4
//#define TRUE 5
//#define FALSE 6
#define INTEGER 7

//	VariabLes
//#define IDENTIFIER 8

// Simbolos
//#define COMA 9
//#define PUNTO 10
//#define DOSPUNTOS 11
//#define PARABRE 12
//#define PARCIERRA 13
//#define SUMA 14
//#define RESTA 15
//#define MULT 16
//#define DIV 17
//#define MOD 18
//#define CONJUNCION 19
//#define DISYUNCION 20
//#define NEGACION 21
//#define MENOR 22
//#define MENORIGUAL 23
//#define MAYOR 24
//#define MAYORIGUAL 25
//#define IGUAL 26

// Reservadas
//#define CREATE 27
//#define WHILE 28
//#define BOOL 29
//#define IF 30
//#define ELSE 31
//#define INT 32
//#define ON 33
//#define END 34
//#define EXECUTE 35
//#define ACTIVATION 36
//#define DEACTIVATION 37
//#define DEFAULT 38
//#define BOT 39
//#define ACTIVATE 40
//#define DEACTIVATE 41
//#define ADVANCE 42

// Entreada y salida
//#define READ 43
//#define SEND 44
//#define RECIEVE 45 
//#define STORE 46


// Movimientos
//#define COLLECT 47
//#define DROP 48
//#define LEFT 49
//#define RIGHT 50
//#define UP 51
//#define DOWN 52

// Especial
#define ME 53

// Comentarios
#define COMENTARIO 54

// Error
//#define ERROR 100


class Token {
	public:
		int identificador;
		string contenido;
};