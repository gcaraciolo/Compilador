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

/**
 * O main eh o unico nao terminal que chama bloco, e que deve
 * deve obrigatoriamente ter um abre chaves e fecha chaves.
 * A variavel verifyBlock
 *
 */
void programa (){
    int verifyBlock;
    
    if (INT == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage("A funcao main deve ser do tipo int");
    }
    if (MAIN == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage("Nao foi achada uma referencia para main");
    }
    if (ABRE_PARENTESES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage("esperado '('");
    }
    if (FECHA_PARENTESES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage("esperado ')'");
    }

    verifyBlock = bloco();
    
    if (-1 == verifyBlock) {
        errorMessage("esperado '{'");
    }else if (-2 == verifyBlock){
        errorMessage("esperado '}'");
    }
    
    if (END_OF_FILE != token.symbol) {
        errorMessage("programa so deve conter a funcao main");
    }
}


/**
 * O retorno da funcao serve apenas para o main.
 * (no if, while) nao eh obrigatorio ter bloco.
 * @return -1 se for erro de nao abrir chaves
 * @return -2 se for erro de nao fechar chaves
 */
int bloco(){
    if (ABRE_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        return -1;
    }
    
    
    decl_var();
    comando();
    
    
    if (FECHA_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        return -2;
    }
    
    return 0;
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
                        errorMessage("esperado ';' no final da declaracao");
                    }
                } else {
                    errorMessage("esperado identificador");
                }
            }
        } else {
            errorMessage("espero ';' no final da declaracao");
        }
    } else {
        errorMessage("esperado identificador");
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

void expr_arit(){
    
}

void atribuicao(){
    if (ID == token.symbol) {
        token = _SCAN();
        if (IGUAL_ATRIBUICAO == token.symbol) {
            token = _SCAN();
            expr_arit();
        }
    }
    
}

void comando_basico(){
    atribuicao();
    bloco();
}

void expr_relacional(){
    
}

void iteracao(){
    switch (token.symbol) {
        case WHILE:
            token = _SCAN();
            if (ABRE_PARENTESES == token.symbol) {
                token = _SCAN();
                expr_relacional();
                if (FECHA_PARENTESES == token.symbol) {
                    token = _SCAN();
                    bloco();
                } else {
                    errorMessage("esperado '}'");
                }
            } else {
                errorMessage("esperado '{'");
            }
            break;
        case DO:
            token = _SCAN();
            bloco();
            if (WHILE == token.symbol) {
                token = _SCAN();
                if (ABRE_PARENTESES == token.symbol) {
                    token = _SCAN();
                    expr_relacional();
                    if (FECHA_PARENTESES == token.symbol) {
                        token = _SCAN();
                        if (PONTO_VIRGULA == token.symbol) {
                            token = _SCAN();
                        } else {
                            errorMessage("esperado ';' no final da expressao");
                        }
                    } else {
                        errorMessage("esperado ')'");
                    }
                    
                } else {
                    errorMessage("esperado '('");
                }
            } else {
                errorMessage("esperado 'while' em do/while loop");
            }
            break;
        default:
            break;
    }
}

void condicional(){
    switch (token.symbol) {
        case IF:
            token = _SCAN();
            if (ABRE_PARENTESES == token.symbol) {
                token = _SCAN();
                expr_relacional();
                if (FECHA_PARENTESES == token.symbol) {
                    token = _SCAN();
                    comando();
                } else {
                    errorMessage("esperado ')'");
                }
            } else {
                errorMessage("esperado '('");
            }
            break;
            
        default:
            break;
    }
    
}

void comando(){
    comando_basico();
    iteracao();
    condicional();
}













