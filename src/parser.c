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

    bloco(true);
    
    if (END_OF_FILE != token.symbol) {
        errorMessage("programa so deve conter a funcao main");
    }
}

boolean isTerminal(){
    switch (token.symbol) {
        case ELSE:
        case DIGITO:
        case DIGITO_FLUTUANTE:
        case LETRA:
        case ABRE_PARENTESES:
        case FECHA_PARENTESES:
        case PONTO_VIRGULA:
        case MAIN:
        case SOMA:
        case SUBTRACAO:
        case DIVISAO:
        case MULTIPLICACAO:
        case MENOR:
        case MAIOR:
        case MENOR_IGUAL:
        case MAIOR_IGUAL:
        case IGUAL_COMPARACAO:
        case DIFERENTE_COMPARACAO:
        case END_OF_FILE:
        case FECHA_CHAVES:
            return true;
            
    }
    return false;
}

void bloco(boolean required){
    boolean open, closed;
    open = closed = false;
    if (required) {
        if (ABRE_CHAVES == token.symbol) {
            token = _SCAN();
        } else {
            errorMessage("esperado '{'");
        }
    } else if (ABRE_CHAVES == token.symbol) {
        token = _SCAN();
        open = true;
    } else {
        return;
    }
    
    
    decl_var();
    while (!isTerminal()){
        comando();
    }
    
    if (required) {
        if (FECHA_CHAVES == token.symbol) {
            token = _SCAN();
        } else {
            errorMessage("esperado '}'");
        }
    } else if (FECHA_CHAVES == token.symbol) {
        token = _SCAN();
        closed = true;
    }
    
    if(open && !closed){
        errorMessage("esperado '}'");
    }
    
    
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


void expressao(){
    termo();
    while (SOMA == token.symbol || SUBTRACAO == token.symbol) {
        token = _SCAN();
        termo();
    }
}

void atribuicao(){
    if (ID == token.symbol) {
        token = _SCAN();
        if (IGUAL_ATRIBUICAO == token.symbol) {
            token = _SCAN();
            expressao();
            if (PONTO_VIRGULA == token.symbol) {
                token = _SCAN();
            } else {
                errorMessage("esperado `;'");
            }
        }
    }    
}

void comando_basico(){
    atribuicao();
    bloco(false);
}

boolean isExpressaoRelacional(){
    switch (token.symbol) {
        case MENOR:
        case MAIOR:
        case MENOR_IGUAL:
        case MAIOR_IGUAL:
        case IGUAL_COMPARACAO:
        case DIFERENTE_COMPARACAO:
            return true;
    }
    return false;
}

void expr_relacional(){
    expressao();
    if (isExpressaoRelacional()) {
        token = _SCAN();
        expressao();
    } else {
        errorMessage("esperado uma expressao relacional");
    }
}


void termo(){
    fator();
    while (MULTIPLICACAO == token.symbol || DIVISAO == token.symbol) {
        token = _SCAN();
        fator();
    }
}

void fator(){
    switch (token.symbol) {
        case ID:
            token = _SCAN();
            break;
        case DIGITO:
        case DIGITO_FLUTUANTE:
        case LETRA:
            token = _SCAN();
            break;
        case ABRE_PARENTESES:
            token = _SCAN();
            expressao();
            if (FECHA_PARENTESES == token.symbol) {
                token = _SCAN();
            } else {
                errorMessage("esperado `)'");
            }
            
            break;
        default:
            errorMessage("esperado um identificador");
    }
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
                    comando();
                } else {
                    errorMessage("esperado ')'");
                }
            } else {
                errorMessage("esperado '('");
            }
            break;
        case DO:
            token = _SCAN();
            comando();
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
                    if (ELSE == token.symbol) {
                        token = _SCAN();
                        comando();
                    }
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













