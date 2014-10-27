#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "symbols.h"
#include "errors.h"
#include "stack.h"

void parser(){
    token = _SCAN();
    scope = -1;
    programa();
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

    symbols_table = stack_create();
    bloco();
    stack_free(&symbols_table);
    
    if (END_OF_FILE != token.symbol) {
        errorMessage("programa so deve conter a funcao main");
    }
    
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

void bloco(){
    scope++;
    
    if (ABRE_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage("esperado '{'");
    }
    
    
    decl_var(&symbols_table);
    while (isCommandFirst()){
        comando();
    }
    
    if (FECHA_CHAVES == token.symbol) {
        token = _SCAN();
    } else {
        errorMessage("esperado '}'");
    }
    
    stack_free_scope(&symbols_table, scope);
    scope--;
    
}

void mult_variables(__STACK ** table, int type){
    char msg[MAX_CHARACTER + 100];
    
    if (ID == token.symbol) {
        if (!stack_consult_scope(*table, scope, token)) {
            stack_push(*table, token, type, scope);
        } else {
            strcpy(msg, "multiplas definicoes para \'");
            strcat(msg, token.lexema);
            strcat(msg, "\'");
            errorMessage(msg);
        }

        token = _SCAN();
        if (PONTO_VIRGULA == token.symbol) {
            token = _SCAN();
        } else if (VIRGULA == token.symbol) {
            token = _SCAN();
            while (true) {
                if (ID == token.symbol) {
                    stack_push(*table, token, type, scope);
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

int getType(){
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

void decl_var(__STACK ** table){
    int type;
    
    do {
        type = getType();
        
        if (UNKNOW_TYPE != type) {
            token = _SCAN();
            mult_variables(table, type);
        }
        
    } while (UNKNOW_TYPE != type);
    
}


void expressao_linha(){
    if(SOMA == token.symbol || SUBTRACAO == token.symbol){
        token = _SCAN();
        termo();
        expressao_linha();
    }
}


void expressao(){
    termo();
    expressao_linha();
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
    if (ID == token.symbol){
        atribuicao();
    } else if (ABRE_CHAVES == token.symbol){
        bloco();
    }
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

void termo_linha(){
   if (MULTIPLICACAO == token.symbol || DIVISAO == token.symbol){
	token = _SCAN();
	fator();
   	termo_linha();
   }
}

void termo(){
    fator(); 
    termo_linha();
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
		    if(isCommandFirst()){
                        comando();
                    } else {
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
	    if(isCommandFirst()){
                comando();
            } else {
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
		    if(isCommandFirst()){
                        comando();
                    } else {
                        errorMessage("esperado um comando");
		    }
                    if (ELSE == token.symbol) {
                        token = _SCAN();
		        if(isCommandFirst()){
                            comando();
                        } else {
                             errorMessage("esperado um comando");
		        }
                    }
                } else {
                    errorMessage("esperado ')'");
                }
            } else {
                errorMessage("esperado '('");
            }
            break;
    }
}

void comando(){
     comando_basico();
     iteracao();
     condicional();
}













