BOT
Cuarta entrega
Traductores e Interpretadores

Integrantes: 
- Gabriel Giménez, carnet 12-11006
- Guillermo Betanccourt, carnet 11-10103

En la presente y ultimate fase el proyecto, se finaliza el interprete del lenguaje BOT.

Verificando que el programa reporte correctamento los siguientes errores:
   - "Uso Inadecuado"
      No se puede utilizar el valor de un robot que:
         1) No esté inicializado con algún valor.
         2) No esté activo.
   - "Activacion Ilegal"
   - "Desactivacion Ilegal"
   - "Division por cero"
   - "Lectura inadecuada"
   - "Coleccion inadecuada"
   - "Almacenamiento inadecuado"

Por otro lado también se verificó que corrieran correctamente los casos de prueba colocados en el grupo del curso.
   - a b.bot
   - exp.bot
   - factorial.bot
   - fibonacci.bot
   - hola-mundo.bot
   - sort.bot
   - sum.bot

Para la implementación de la matriz de los robots se utilizó un map<int, map<int, variable *> > 
Donde variable es una clase utilizada para manejar todos los tipos (booleanos, int, char) como una sola clase.

Por otro lado se creó la clase Robot para llevar el manejo de activaciones / desactivaciones y manejo de posiciones en la matriz.

Finalmente se arreglaron errores detectados en las entregas pasadas.

Como se indica en el enunciado, el proyecto funciona correctamente en las computadoras del LDC (Linux).