#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "symbols.h"
#include "globals.h"
#include "messages.h"

boolean verifyLookahead(char lookahead){
    switch (lookahead) {
        case SPACE:
        case NEW_LINE:
        case NEW_LINE2:
        case TAB_:
        case S_MENOR:
        case S_MAIOR:
        case S_IGUAL:
        case S_MULTIPLICACAO:
        case S_SUBTRACAO:
        case S_DIVISAO:
        case S_SOMA:
        case S_ABRE_CHAVES:
        case S_FECHA_CHAVES:
        case S_ABRE_PARENTESES:
        case S_FECHA_PARENTESES:
        case S_EXCLAMACAO:
        case S_VIRGULA:
        case S_PONTO_VIRGULA:
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
    
    while (isspace(lookahead)) {
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
                lookahead = readCharacter();
                break;
            case S_SOMA:
                token.symbol = SOMA;
                lookahead = readCharacter();
                break;
            case S_SUBTRACAO:
                token.symbol = SUBTRACAO;
                lookahead = readCharacter();
                break;
            case S_IGUAL:
                token.symbol = IGUAL_ATRIBUICAO;
                lookahead = readCharacter();
                ///mais coisas
                break;
            case S_DIVISAO:
                token.symbol = DIVISAO;
                lookahead = readCharacter();
                ///mais coisas
                break;
                
                
            case S_EXCLAMACAO:
                lookahead = readCharacter();
                token.lexema[pointer++] = lookahead;
                if (S_IGUAL == lookahead) {
                    token.symbol = DIFERENTE_COMPARACAO;
                    lookahead = readCharacter();                    
                }else{
                    token.symbol = ERROR_OPERADOR_MAL_FORMADO; //erro
                }
                break;

            default:
                if (true == isdigit(lookahead)) {
                    token.symbol = DIGITO;
                } else if (true == isalpha(lookahead) || S_UNDERLINE == lookahead){
                    /* identificador */
                    while (true){
                        if (MAX_CHARACTER == (pointer - 1)) {
                            token.symbol = ERROR_VARIABLE_TOO_LONG;
                            break;
                        }
                        lookahead = readCharacter();
                        //verify lexema's end.
                        if(verifyLookahead(lookahead)){
                            //verify if is a keyword
                            token.lexema[pointer] = '\0';
                            if (!verifyKeyword(token.lexema)){
                                //not keyword
                                token.symbol = ID;
                            }else{
                                token.symbol = verifyKeyword(token.lexema);
                            }
                            break;
                        }
                        if (true == isalnum(lookahead) || S_UNDERLINE == lookahead) {
                            token.lexema[pointer++] = lookahead;
                        } else {
                            break;
                        }
                    }

                } else{
                    token.symbol = ERROR_UNKNOW_SYMBOL;
                }
        }
    }
    token.lexema[pointer] = '\0';
    return token;
}

void printToken(__TOKEN token){
    if (token.symbol > 29 && END_OF_FILE != token.symbol) {
        printf ("lexema desconhecido: ");
        printf ("\nsymbol: %d\n\n",token.symbol);
    } else {
        printf("lexema: %s\n",token.lexema);
        printf("symbol: %d\n\n",token.symbol);
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
    linha = 1;
    coluna = 0;
    while(true){
        token = _SCAN();
        if(verifyToken(token)){
            break;
        }else{
            printToken(token);
        }
        if(token.symbol == END_OF_FILE){
            break;
        }
    }
}













