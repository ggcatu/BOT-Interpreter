SintBot: scanner.o lex.yy.c parser.tab.h definiciones.o
	g++ scanner.o definiciones.o lex.yy.c parser.tab.c -o SintBot

scanner.o: scanner.cpp parser.tab.h
	g++ -c scanner.cpp definiciones.cpp

definiciones.o:
	g++ -c definiciones.cpp

lex.yy.c:
	flex lexer.l

parser.tab.h:
	bison -d parser.y

clean:
	rm lex.yy.c parser.tab.c parser.tab.h scanner.o definiciones.o
