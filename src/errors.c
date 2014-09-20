//
//  errors.c
//  Compilador
//
//  Created by Guilherme Caraciolo on 17/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "messages.h"
#include "globals.h"

void errorMessage(const char *message){
    if (strlen(token.lexema) == 0) {
        printf("ERRO na linha %d, coluna %d, nenhum token lido: %s\n", linha, coluna, message);
    }else {
        printf("ERRO na linha %d, coluna %d, ultimo token lido '%s': %s\n", linha, coluna, token.lexema, message);
    }
    exit(1);
}

void verifyToken(__TOKEN token){
    if(ERROR_OPERADOR_MAL_FORMADO == token.symbol){
        errorMessage("operador != mal formado");
    } else if (ERROR_CHAR_MAL_FORMADO == token.symbol){
        errorMessage("caracter mal formado");
    } else if (ERROR_FLOAT_MAL_FORMADO == token.symbol){
        errorMessage("float mal formado");
    } else if (ERROR_UNKNOW_SYMBOL == token.symbol){
        errorMessage("caracter invalido");
    } else if (ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO == token.symbol){
        errorMessage("comentario nao encerrado.");
    }
}