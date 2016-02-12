bot: scanner.o lex.yy.c parser.tab.h
	g++ scanner.o lex.yy.c parser.tab.c -o bot

scanner.o: scanner.cpp parser.tab.h
	g++ -c scanner.cpp definiciones.cpp

lex.yy.c:
	flex lexer.l

parser.tab.h:
	bison -d parser.y

clean:
	rm lex.yy.c parser.tab.c parser.tab.h scanner.o
