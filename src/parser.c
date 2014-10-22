#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "symbols.h"
#include "errors.h"

boolean parser(){
    boolean executed = false;
    token = _SCAN();
    executed = true;
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
    return executed;
}

boolean programa (){
    boolean executed = false, requiredChaves = true;
    if (INT == token.symbol) {
        token = _SCAN();
        executed = true;
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

    bloco(requiredChaves);
    
    if (END_OF_FILE != token.symbol) {
        errorMessage("programa so deve conter a funcao main");
    }
    
    return executed;
}

boolean isCommandFirst(){
    switch (token.symbol) {
        case ID:
        case ABRE_CHAVES:
        case WHILE:
        case DO:
        case IF:
            return true;
    }
    return false;
}

boolean bloco(boolean required){
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
        return false;
    }
    
    
    decl_var();
    while (isCommandFirst()){
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
    return true;
}

boolean mult_variables(){
    boolean executed = false;
    if (ID == token.symbol) {
        token = _SCAN();
        executed = true;
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
    return executed;
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

boolean decl_var(){
    int tipo;
    boolean executed = false;
    do {
        tipo = getTipo();
        switch (tipo) {
            case INT:
            case CHAR:
            case FLOAT:
                token = _SCAN();
                executed = true;
                mult_variables();
                break;
            default:
                break;
        }
    } while (UNKNOW_TYPE != tipo);
    return executed;
}


boolean expressao_linha(){
    boolean executed = false;
    if(SOMA == token.symbol || SUBTRACAO == token.symbol){
	token = _SCAN();
	executed = true;
	termo();
	expressao_linha();	
    }
    return executed;
}


boolean expressao(){
    termo();
    expressao_linha();
}

boolean atribuicao(){
    boolean executed = false;
    if (ID == token.symbol) {
        token = _SCAN();
        executed = true;
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
    return executed;
}

boolean comando_basico(){
    boolean executed = false, requiredChaves = false;
    executed = atribuicao();
    if (!executed) {
        executed = bloco(requiredChaves);
    }
    return executed;
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

boolean expr_relacional(){
    boolean executed = false;
    executed = expressao();
    if (isExpressaoRelacional()) {
        token = _SCAN();
        executed = true;
        expressao();
    } else {
        errorMessage("esperado uma expressao relacional");
    }
    return executed;
}

boolean termo_linha(){
   boolean executed = false;
   if (MULTIPLICACAO == token.symbol || DIVISAO == token.symbol){
	token = _SCAN();
	executed = true;
	fator();
   	termo_linha();
   }
   return executed;
}

boolean termo(){
    fator(); 
    termo_linha();
}

boolean fator(){
    boolean executed = false;
    switch (token.symbol) {
        case ID:
            token = _SCAN();
            executed = true;
            break;
        case DIGITO:
        case DIGITO_FLUTUANTE:
        case LETRA:
            token = _SCAN();
            executed = true;
            break;
        case ABRE_PARENTESES:
            token = _SCAN();
            executed = true;
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
    return executed;
}

boolean iteracao(){
    boolean executed = false;
    switch (token.symbol) {
        case WHILE:
            token = _SCAN();
            executed = true;
            if (ABRE_PARENTESES == token.symbol) {
                token = _SCAN();
                expr_relacional();
                if (FECHA_PARENTESES == token.symbol) {
                    token = _SCAN();
                    if(!comando()){
			errorMessage("esperado um comando");	
		    }
                } else {
                    errorMessage("esperado ')'");
                }
            } else {
                errorMessage("esperado '('");
            }
            break;
        case DO:
            token = _SCAN();
            executed = true;
            if(!comando()){
		errorMessage("esperado um comando");
	    }
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
    }
    return executed;
}

boolean condicional(){
    boolean executed = false;
    switch (token.symbol) {
        case IF:
            token = _SCAN();
            executed = true;
            if (ABRE_PARENTESES == token.symbol) {
                token = _SCAN();
                expr_relacional();
                if (FECHA_PARENTESES == token.symbol) {
                    token = _SCAN();
                    if (!comando()){
			errorMessage("esperado um comando");	
		    }
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
    }
    return false;
}

boolean comando(){
    boolean executed = false;
    executed = comando_basico();
    if(!executed){
        executed = iteracao();
    }
    if (!executed) {
        executed = condicional();
    }
    return executed;
}













