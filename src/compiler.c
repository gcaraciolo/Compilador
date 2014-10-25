#include <stdio.h>
#include "fileActions.h"
#include "parser.h"
#include "messages.h"

int main(int argc, char *argv[]){
    int statusFile;
	
    
	/*if(argc < 2){
		printf ("Parameters not passed.\n");
		return ERROR_PARAMETERS_NOT_PASSED;
	}*/
    
    argv[1] = "/Users/gcaraciolo/Documents/Compilador/test/parser/programa";
    
	statusFile = openFile(argv[1]);
	if(ERROR_COULD_NOT_OPEN_FILE == statusFile){
		printf ("Could not open file.\n");
		return ERROR_COULD_NOT_OPEN_FILE;
	}
    
    linha = 1;
    coluna = 0;
    
    parser();


	statusFile = closeFile();
	if(ERROR_FILE_CLOSED_UNSECCESSFULLY == statusFile){
		printf ("File closed unseccessfully.\n");
		return ERROR_FILE_CLOSED_UNSECCESSFULLY;
	}
    
    printf("Build succeeded.\n");

	return 0;
}
