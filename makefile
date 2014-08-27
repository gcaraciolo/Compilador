all: compiler


compiler: symbols
	gcc compiler.c -o compiler

symbols: compiler.c
	gcc compiler.c symbols.h

