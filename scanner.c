#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "symbols.h"
#include "globals.h"
#include "messages.h"

boolean verifyLookahead(char lookahead){
    if (lookahead == SPACE || lookahead == NEW_LINE || lookahead == TAB_ ) {
        return true;
    }
    return false;
}

boolean verifyKeyword(const char *str){
    if (strcmp(str, S_MAIN) == 0) {
        return MAIN;
    }else if (strcmp(str, S_IF) == 0){
        return IF;
    }else if (strcmp(str, S_ELSE) == 0){
        return ELSE;
    }else if (strcmp(str, S_WHILE) == 0){
        return WHILE;
    }else if (strcmp(str, S_DO) == 0){
        return DO;
    }else if (strcmp(str, S_FOR) == 0){
        return FOR;
    }else if (strcmp(str, S_INT) == 0){
        return INT;
    }else if (strcmp(str, S_FLOAT) == 0){
        return FLOAT;
    }else if (strcmp(str, S_CHAR) == 0){
        return CHAR;
    }   
    return false;
}

char readCharacter(){
    char lookahead = fgetc(file);
    if (NEW_LINE == lookahead) {
        linha++;
        coluna = 0;
    }else if(TAB_ == lookahead){
        coluna += 4;
    }else{
        coluna++;
    }
    return lookahead;
}

__TOKEN _SCAN(){
	static char lookahead = SPACE;
    __TOKEN token;
    int pointer = 0;
    
    while (verifyLookahead(lookahead)) {
        lookahead = readCharacter();
    }
    
    if (EOF == lookahead) {
        token.lexema[pointer++] = lookahead;
        token.symbol = END_OF_FILE;
    } else {
        token.lexema[pointer++] = lookahead;
        switch (lookahead) {
            /* oparitimeticos */
            case S_MULTIPLICACAO:
                token.symbol = MULTIPLICACAO;
                lookahead = fgetc(file);
                break;
            case S_SOMA:
                token.symbol = SOMA;
                lookahead = fgetc(file);
                break;
            case S_SUBTRACAO:
                token.symbol = SUBTRACAO;
                lookahead = fgetc(file);
                break;
            case S_IGUAL:
                token.symbol = IGUAL_ATRIBUICAO;
                ///mais coisas
                break;
            case S_DIVISAO:
                token.symbol = DIVISAO;
                ///mais coisas
                break;
                
                
            case S_EXCLAMACAO:
                lookahead = readCharacter();
                token.lexema[pointer++] = lookahead;
                if (S_IGUAL == lookahead) {
                    token.symbol = DIFERENTE_COMPARACAO;
                }else{
                    token.symbol = ERROR_OPERADOR_MAL_FORMADO; //erro
                }
                break;

            default:
                if (true == isdigit(lookahead)) {
                    token.symbol = DIGITO;
                } else if (true == isalpha(lookahead) || S_UNDERLINE == lookahead){
                    /* identificador */
                    do {
                        if (MAX_CHARACTER == (pointer - 1)) {
                            token.symbol = ERROR_VARIABLE_TOO_LONG;
                            break;
                        }
                        lookahead = fgetc(file);
                        if(verifyLookahead(lookahead)){
                            //verify if is a keyword
                            token.lexema[pointer] = '\0';
                            if (verifyKeyword(token.lexema) == false){
                                //not keyword
                                token.symbol = ID;
                            }else{
                                token.symbol = verifyKeyword(token.lexema);
                            }
                            break;
                        }
                        token.lexema[pointer++] = lookahead;
                    } while (true == isalnum(lookahead) || S_UNDERLINE == lookahead);

                } else{
                    token.symbol = ERROR_UNKNOW_SYMBOL;
                }
        }
    }
    token.lexema[pointer] = '\0';
    return token;
}

void printToken(__TOKEN token){
    if (token.symbol > 29) {
        printf ("lexema desconhecido: ");
        printf ("\nsymbol: %d\n",token.symbol);
    }
}

void errorMessage(const char *message){
    printf("ERRO na linha %d, coluna %d, ultimo token lido t: %s\n", linha, coluna, message);
}

boolean verifyToken(__TOKEN token){
    if(ERROR_OPERADOR_MAL_FORMADO == token.symbol){
        errorMessage("operador != mal formado");
        return true;
    }
    
    return false;
}

void readFile(){
    __TOKEN token;
    linha = coluna = 0;
    while(true){
        token = _SCAN();
        if(verifyToken(token)){
            break;
        }
        if(token.symbol == END_OF_FILE){
            break;
        }
    }
}













