#include <stdio.h>
#include <string.h>
#include "fileActions.h"
#include "messages.h"
#include "globals.h"

int openFile (const char *name) {
    file = fopen (name,"rb"); // file open just for read
    if (NULL == file) { // could not open file
        	return ERROR_COULD_NOT_OPEN_FILE;
    }
    return FILE_OPENED_SUCCESSFULLY;
}


int closeFile () {
        int status;
        status = fclose (file);
        if (0 == status){
            return FILE_CLOSED_SUCCESSFULLY;
        }else{
            return ERROR_FILE_CLOSED_UNSECCESSFULLY;
        }
}

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