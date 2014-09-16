#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"


 __TOKEN token;

void programa (){
    
    if (INT == token.symbol) {
        token = _SCAN();
        if (MAIN == token.symbol) {
            token = _SCAN();
            if (ABRE_PARENTESES == token.symbol) {
                token = _SCAN();
                if (FECHA_PARENTESES == token.symbol) {
                    token = _SCAN();
                    //chama bloco
                } else {
                    //erro
                }
            } else {
                
            }
        } else {
            
        }
    } else {
        
    }
    
}

void parser(){
   
    linha = 1;
    coluna = 0;
    
    token = _SCAN();
    programa();
    
    
    /*
    while(true){
        
        if(END_OF_FILE == token.symbol){
            printf("Build succeeded.\n");
            break;
        }
    }*/
}


