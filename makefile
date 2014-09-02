program: compiler.o fileActions.o scanner.o test
	gcc -o bin/compiler compiler.o fileActions.o scanner.o

compiler.o: compiler.c scanner.h symbols.h messages.h fileActions.h globals.h
	gcc -c compiler.c scanner.h symbols.h messages.h fileActions.h globals.h

fileActions.o: fileActions.c fileActions.h messages.h globals.h
	gcc -c fileActions.c fileActions.h messages.h globals.h

scanner.o: scanner.c scanner.h symbols.h globals.h messages.h fileActions.h
	gcc -c scanner.c symbols.h globals.h messages.h fileActions.h

test: test.o fileActions.o scanner.o
	gcc -o bin/test test.o fileActions.o scanner.o

test.o: test/test.c scanner.h
	gcc -c test/test.c scanner.h

clean:
	rm compiler.o fileActions.o scanner.o \
	   messages.h.gch globals.h.gch fileActions.h.gch scanner.h.gch symbols.h.gch \
	   test.o