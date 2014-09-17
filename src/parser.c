#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "symbols.h"
#include "errors.h"

void programa (){
    if (INT == token.symbol) {
        token = _SCAN();
    } else {
        errorParser("");
    }
    if (MAIN == token.symbol) {
        token = _SCAN();
    } else {
        errorParser("");
    }
    if (ABRE_PARENTESES == token.symbol) {
        token = _SCAN();
    } else {
        errorParser("");
    }
    if (FECHA_PARENTESES == token.symbol) {
        token = _SCAN();
    } else {
        errorParser("");
    }
    
}

void parser(){
    //token = _SCAN();
    //programa();
    
    
    
    while(true){
        token = _SCAN();
        if(END_OF_FILE == token.symbol){
           // printf("Build succeeded.\n");
            break;
        }
       // printToken(token);
    }
}


