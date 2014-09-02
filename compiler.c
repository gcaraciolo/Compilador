#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "symbols.h"
#include "messages.h"
#include "fileActions.h"
#include "globals.h"

int main(int argc, char *argv[]){
	int statusFile;
	
	
	if(argc < 2){
		printf ("Parameters not passed.\n");
		return ERROR_PARAMETERS_NOT_PASSED;
	}
	
	//argv[1] = "/Users/gcaraciolo/Documents/Projetos/Compilador/errors_test_scanner.txt";

	if(!verifyExtension(argv[1])){
		printf ("Extension not recognized.\n");
		return ERROR_FILE_NOT_RECOGNIZED;
	}

	statusFile = openFile(argv[1]);
	if(ERROR_COULD_NOT_OPEN_FILE == statusFile){
		printf ("Could not open file.\n");
		return ERROR_COULD_NOT_OPEN_FILE;
	}

	readFile();


	statusFile = closeFile();
	if(ERROR_FILE_CLOSED_UNSECCESSFULLY == statusFile){
		printf ("File closed unseccessfully.\n");
		return ERROR_FILE_CLOSED_UNSECCESSFULLY;
	}

	return 0;
}
