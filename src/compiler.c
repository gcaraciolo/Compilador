#include <stdio.h>
#include "fileActions.h"
#include "parser.h"
#include "messages.h"

int main(int argc, char *argv[]){
	int statusFile;
	
    /*
	if(argc < 2){
		printf ("Parameters not passed.\n");
		return ERROR_PARAMETERS_NOT_PASSED;
	}*/
	//test scanner
	//argv[1] = "/Users/gcaraciolo/Documents/Projetos/Compilador/test/scanner.txt";
    //test main
    //argv[1] = "/Users/gcaraciolo/Documents/Projetos/Compilador/test/parser/programa.txt";
    //test declaracao variavel
    //argv[1] = "/Users/gcaraciolo/Documents/Projetos/Compilador/test/parser/decl_var.txt";
    //test comando
    //argv[1] = "/Users/gcaraciolo/Documents/Projetos/Compilador/test/parser/comando.txt";
    //test iteracao
    argv[1] = "/Users/gcaraciolo/Documents/Projetos/Compilador/test/parser/iteracao.txt";

	if(!verifyExtension(argv[1])){
		printf ("Extension not recognized.\n");
		return ERROR_FILE_NOT_RECOGNIZED;
	}

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
