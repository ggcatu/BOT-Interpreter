ContBot: scanner.o robot.o lex.yy.c parser.tab.c definiciones.o ast.h ast.cpp
	g++ scanner.o definiciones.o robot.o lex.yy.c parser.tab.c ast.cpp -o ContBot

scanner.o: scanner.cpp parser.tab.h
	g++ -c scanner.cpp definiciones.cpp

definiciones.o:
	g++ -c definiciones.cpp

robot.o: tabla_simbolos.h arb_def.h robot.h robot.cpp variables.h
	g++ -c robot.cpp

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm lex.yy.c parser.tab.c parser.tab.h scanner.o definiciones.o
