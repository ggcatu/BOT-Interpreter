BOT
Segunda entrega
Traductores e Interpretadores

Integrantes: - Gabriel Giménez, carnet 12-11006
			 - Guillermo Betanccourt, carnet 11-10103

En la presente fase el proyecto, se implementa un analizador sintáctico para el lenguaje BOT.

A nivel gramático, en el archivo parser.y, se implementa una gramática libre de contexto que nos permite reconocer las instrucciones del lenguaje. Básicamente se separa la gramática en las declaraciones y en la ejecución. Las declaraciones no son impresas por la salida estándar.

En la ejecución, se tienen las distintas expresiones que el lenguaje debe reconocer, sin embargo, en esta fase el proyecto no se realiza verificación de tipos para los operandos de las instrucciones, solamente se reconoce si una instrucción está formada adecuadamente a nivel sintáctico.

En el archivo ast.h encontramos todas las declaraciones de las clases utilizadas para poder crear nuestro Árbol Sintáctico Abstracto. Nos valemos de las técnicas de la programación orientada a objetos (OOP), para crear clases que heredan de una clase base, y del polimorfismo, para poder construir el árbol sin saber exactamente qué tipo de hijo estamos esperando.