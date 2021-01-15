CC = gcc




all: test_parser.x test_lexer.x



test_parser.x: test_parser.c parser.o lexer.o
	$(CC) test_parser.c parser.o lexer.o -o test_parser.x

test_lexer.x: test_lexer.c lexer.o
	$(CC) test_lexer.c lexer.o -o test_lexer.x


parser.o: ../parser.c parser.h 
	$(CC) -c ../parser.c -o parser.o


lexer.o: ../lexer.c ../lexer.h
	$(CC) -c ../lexer.c -o lexer.o

parser.h: ../parser.h
	touch ../parser.h
lexer.h: ../lexer.h
	touch ../lexer.h
