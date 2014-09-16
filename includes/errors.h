//
//  errors.h
//  Compilador
//
//  Created by Guilherme Caraciolo on 15/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#ifndef Compilador_errors_h
#define Compilador_errors_h

#include <stdio.h>
#include "globals.h"
#include "messages.h"

void errorMessage(__TOKEN token, const char *message){
    printf("ERRO na linha %d, coluna %d, ultimo token lido %s: %s\n", linha, coluna, token.lexema, message);
}

boolean verifyToken(__TOKEN token){
    if(ERROR_OPERADOR_MAL_FORMADO == token.symbol){
        errorMessage(token, "operador != mal formado");
        return false;
    } else if (ERROR_CHAR_MAL_FORMADO == token.symbol){
        errorMessage(token, "caracter mal formado");
        return false;
    } else if (ERROR_FLOAT_MAL_FORMADO == token.symbol){
        errorMessage(token, "float mal formado");
        return false;
    } else if (ERROR_UNKNOW_SYMBOL == token.symbol){
        errorMessage(token, "caracter invalido");
        return false;
    } else if (ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO == token.symbol){
        errorMessage(token, "comentario nao encerrado.");
        return false;
    }
    return true;
}


#endif
