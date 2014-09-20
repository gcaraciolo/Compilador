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
    comando();
    
    
    if (FECHA_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage(token, "esperado '}'");
    }
    
   // token = _SCAN();
}

void mult_variables(){
    if (ID == token.symbol) {
        token = _SCAN();
        if (PONTO_VIRGULA == token.symbol) {
            token = _SCAN();
        } else if (VIRGULA == token.symbol) {
            token = _SCAN();
            while (true) {
                if (ID == token.symbol) {
                    token = _SCAN();
                    if (VIRGULA == token.symbol) {
                        token = _SCAN();
                    } else if (PONTO_VIRGULA == token.symbol) {
                        token = _SCAN();
                        break;
                    } else {
                        errorMessage(token, "esperado ';' no final da declaracao");
                    }
                } else {
                    errorMessage(token, "esperado identificador");
                }
            }
        }
    } else {
        errorMessage(token, "esperado identificador");
    }
}

int getTipo(){
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

void decl_var(){
    int tipo;
    do {
        tipo = getTipo();
        switch (tipo) {
            case INT:
            case CHAR:
            case FLOAT:
                token = _SCAN();
                mult_variables();
                break;
            default:
                break;
        }
    } while (UNKNOW_TYPE != tipo);
}

void atribuicao(){
    
}

void comando_basico(){
    atribuicao();
}

void comando(){
    comando_basico();
    /*iteracao();
    condicional();   */
}













