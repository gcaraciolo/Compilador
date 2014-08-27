#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "symbols.h"

int verifyExtension(const char *name){
	int s_extensions = 1;
	const char *extensions[s_extensions];	
	int i;
	extensions[0] = ".txt";
	boolean verify = false;
	name = strrchr(name, '.');
	if(name){
		for(i = 0; i < s_extensions; i++){
			verify = strcmp(name, extensions[i]) == 0;
			if(verify){
				return true;
			}
		}
		
	}
	return false;
}


int main(int argc, char *argv[]){
	if(argc < 2){
		printf ("Parameters not passed.\n");
		return ERROR_PARAMETERS_NOT_PASSED;
	}

	if(!verifyExtension(argv[1])){
		printf ("Extension not recognized.\n");
		return ERROR_FILE_NOT_RECOGNIZED;
	}


	return 0;
}