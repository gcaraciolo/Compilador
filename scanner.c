#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "symbols.h"
#include "globals.h"
#include "messages.h"

int _SCAN(){
	char lookahead = SPACE;

	
		lookahead = getc(file);
		printf ("%c", lookahead);
	

	return 0;
}



void readFile(){

	_SCAN();

	printf ("reading file is starting.\n");

}