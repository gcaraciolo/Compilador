//
//  errors.c
//  Compilador
//
//  Created by Guilherme Caraciolo on 17/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "messages.h"

void errorScanner(__TOKEN token, const char *message){
    printf("ERRO na linha %d, coluna %d, ultimo token lido %s: %s\n", linha, coluna, token.lexema, message);
}

boolean verifyToken(__TOKEN token){
    if(ERROR_OPERADOR_MAL_FORMADO == token.symbol){
        errorScanner(token, "operador != mal formado");
        return false;
    } else if (ERROR_CHAR_MAL_FORMADO == token.symbol){
        errorScanner(token, "caracter mal formado");
        return false;
    } else if (ERROR_FLOAT_MAL_FORMADO == token.symbol){
        errorScanner(token, "float mal formado");
        return false;
    } else if (ERROR_UNKNOW_SYMBOL == token.symbol){
        errorScanner(token, "caracter invalido");
        return false;
    } else if (ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO == token.symbol){
        errorScanner(token, "comentario nao encerrado.");
        return false;
    }
    return true;
}


void errorParser(const char *message){
    printf("%s\n", message);
    exit(1);
}