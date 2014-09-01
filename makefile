program: compiler.o fileActions.o scanner.o
	gcc -o compiler compiler.o fileActions.o scanner.o

compiler.o: compiler.c scanner.h symbols.h messages.h fileActions.h globals.h
	gcc -c compiler.c scanner.h symbols.h messages.h fileActions.h globals.h

fileActions.o: fileActions.c fileActions.h messages.h globals.h
	gcc -c fileActions.c fileActions.h messages.h globals.h

scanner.o: scanner.c scanner.h symbols.h globals.h messages.h
	gcc -c scanner.c symbols.h globals.h messages.h

clean:
	rm compiler.o fileActions.o scanner.o \
	   messages.h.gch globals.h.gch fileActions.h.gch scanner.h.gch symbols.h.gch