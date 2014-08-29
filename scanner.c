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

__TOKEN _SCAN(){
	static char lookahead = SPACE;
    int status;
    __TOKEN token;
    int pointer = 0;
    
    while (verifyLookahead(lookahead)) {
        lookahead = fgetc(file);
    }
    
    if (EOF == lookahead) {
        token.lexema[pointer++] = lookahead;
        token.symbol = 100;
    }else{
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
                status = (lookahead = fgetc(file));
                token.lexema[pointer++] = lookahead;
                if (S_IGUAL == lookahead) {
                    token.symbol = DIFERENTE_COMPARACAO;
                }else{
                    token.symbol = 1000; //erro
                }
                break;

            default:
                if (true == isdigit(lookahead)) {
                    token.symbol = DIGITO;
                } else if (true == isalpha(lookahead) || S_UNDERLINE == lookahead){
                    /* identificador */
                    do {
                        if (MAX_CHARACTER == (pointer -1)) {
                            token.symbol = 2000; //erro
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
                    token.symbol = 1000;
                }
        }
    }
    token.lexema[pointer] = '\0';
    return token;
}

void printToken(__TOKEN token){
    int i;
    if (token.symbol == ID) {
        printf ("---------------------\n");
        printf("lexema: %s\n",token.lexema);
        printf("symbol: %d - ID\n",token.symbol);
        printf ("---------------------\n");
    } else if (verifyKeyword(token.lexema)){
        printf ("---------------------\n");
        printf("lexema: %s\n",token.lexema);
        printf("symbol: %d - palavra reservada\n",token.symbol);
        printf ("---------------------\n");
        
    } else if (token.symbol > 29) {
        printf ("---------------------\n");
        printf ("lexema esconhecido: ");
        for (i = 0; i < strlen(token.lexema); i++) {
            if(token.lexema[i] == NEW_LINE){
                printf("newline");
            }else if (token.lexema[i] == TAB_){
                printf("tab");
            }else if (token.lexema[i] == SPACE){
                printf("space");
            }else if (token.lexema[i] == EOF){
                printf("end of file");
            }else{
                printf("%c",token.lexema[i]);
            }
        }
        printf ("\nsymbol: %d\n",token.symbol);
        printf("---------------------\n");
    }

}


void readFile(){
    __TOKEN token;
    while(true){
        token = _SCAN();
        printToken(token);
        if(token.symbol == 100){
            break;
        }
    }
    


}