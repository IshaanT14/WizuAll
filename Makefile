CC = gcc
BISON = bison -d
FLEX = flex

all: vizrunner

vizrunner: grammar.tab.c lex.yy.c symbol_registry.o code_emitter.o
	$(CC) -o vizrunner grammar.tab.c lex.yy.c symbol_registry.o code_emitter.o -lfl

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) grammar.y

lex.yy.c: tokenizer.l grammar.tab.h
	$(FLEX) tokenizer.l

symbol_registry.o: symbol_registry.c symbol_registry.h
	$(CC) -c symbol_registry.c

code_emitter.o: code_emitter.c code_emitter.h symbol_registry.h
	$(CC) -c code_emitter.c

clean:
	rm -f vizrunner parser.tab.* lex.yy.c *.o out.c