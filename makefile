SintBot: scanner.o lex.yy.c parser.tab.h definiciones.o ast.h ast.cpp
	g++ scanner.o definiciones.o lex.yy.c parser.tab.c ast.cpp -o SintBot

scanner.o: scanner.cpp parser.tab.h
	g++ -c scanner.cpp definiciones.cpp

definiciones.o:
	g++ -c definiciones.cpp

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.h: parser.y
	bison -d parser.y

clean:
	rm lex.yy.c parser.tab.c parser.tab.h scanner.o definiciones.o
