#include <stdio.h>
#include "../scanner.h"
#include "../symbols.h"
#include "../messages.h"
#include "../fileActions.h"
#include "../globals.h"

char *route = "test/testFile.txt";


void createFileTest(){
	file = fopen(route, "w+");
   	fputs("!=\n'ss'", file);
   	fclose(file);
}

int main(int argc, char *argv[]){
	int compiler = 0;
	int statusFile;
	
	createFileTest();

	statusFile = openFile(route);
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