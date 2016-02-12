bot: scanner.o lex.yy.c parser.tab.c
	g++ scanner.o lex.yy.c parser.tab.c -o bot

scanner.o: scanner.cpp
	g++ -c scanner.cpp

lex.yy.c:
	flex lexer.l

parser.tab.c:
	bison -d parser.y

clean:
	rm lex.yy.c lex.yy.o parser.output parser.tab.c parser.tab.o parser.tab.h scanner.o
