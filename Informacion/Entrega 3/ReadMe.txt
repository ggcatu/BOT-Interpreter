BOT
Tercera entrega
Traductores e Interpretadores

Integrantes: - Gabriel Giménez, carnet 12-11006
			 - Guillermo Betanccourt, carnet 11-10103

En la presente fase el proyecto, se implementa el chequeo de tipos para el lenguaje BOT.

Verificando que se cumplan las siguientes reglas:
   - La creacion de una tabla de simbolos  para almacenar las variables que han sido declaradas, sus tipos y sus valores.
   - Que los tipos cuadren
   - No usar variables no declaradas
   - No redeclarar variables
   - Que exista a lo sumo una definición de 'on activation' para cada bot.
   - Que exista a lo sumo una definición de 'on deactivation' para cada bot.
   - Que exista a lo sumo una definición de 'on default' para cada bot y, de existir, la misma se encuentra al final de la lista de comportamientos.
   - Que no se use la variable 'me' fuera de una instrucción de robot.
   - En los comportamientos solamente se pueden utilizar las variables locales (por ejemplo, las que se crean con collect) y la variable 'me'. 
	No se puede hacer referencia a ningún otro bot en este contexto.

Aparte de que se verificaran las condiciones implicitas de las funciones ( Expresiones aritmeticas, condiciones booleanas, etc. )

Básicamente solo se modificó el archivo ast.h para incluir la definicion de la clase tabla de símbolos, que nos permite la jerarquización de las tablas
de simbolos, y la inclusión de los métodos de chequeo de tipos que se implementan en las diferentes clases.