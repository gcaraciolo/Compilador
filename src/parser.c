#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "symbols.h"
#include "errors.h"

void parser(){
    token = _SCAN();
    programa();
    
    
    /*
     while(true){
     token = _SCAN();
     if(END_OF_FILE == token.symbol){
     // printf("Build succeeded.\n");
     break;
     }
     // printToken(token);
     }*/
}

void programa (){
    if (INT == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "A funcao main deve ser do tipo int");
    }
    if (MAIN == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "Nao foi achada uma referencia para main");
    }
    if (ABRE_PARENTESES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "esperado '('");
    }
    if (FECHA_PARENTESES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "esperado ')'");
    }

    bloco();
    
    if (END_OF_FILE != token.symbol) {
        errorMessage(token, "");
    }
}

void bloco(){
    
    if (ABRE_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "esperado '{'");
    }
    
    
    decl_var();
    
    
    if (FECHA_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "esperado '}'");
    }
    
    token = _SCAN();
}

void decl_var(){
    switch (tipo()) {
        case INT:
            token = _SCAN();
            if (ID == token.symbol) {
                token = _SCAN();
                if (PONTO_VIRGULA == token.symbol) {
                    token = _SCAN();
                } else if (VIRGULA == token.symbol) {
                    while (true) {
                        if (ID == token.symbol) {
                            token = _SCAN();
                            if (VIRGULA == token.symbol) {
                                token = _SCAN();
                            } else if (PONTO_VIRGULA == token.symbol) {
                                token = _SCAN();
                                break;
                            } else {
                                errorMessage(token, "");
                            }
                        } else {
                            errorMessage(token, "");
                        }
                    }
                }
            } else {
                errorMessage(token, "");
            }
            break;
            
        default:
            break;
    }

    
}

int tipo(){
    switch (token.symbol) {
        case INT:
            return INT;
        case FLOAT:
            return FLOAT;
        case CHAR:
            return CHAR;
    }
    return UNKNOW_TYPE;
}



















