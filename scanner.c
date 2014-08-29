#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "symbols.h"
#include "globals.h"
#include "messages.h"

__TOKEN _SCAN(){
	static char lookahead = SPACE;
    int status;
    __TOKEN token;
    int pointer = 0;
    
    do {
        status = (lookahead = fgetc(file));
    } while (lookahead == SPACE);
    
    if (EOF == status) {
        token.lexema[pointer++] = lookahead;
        token.symbol = 100;
    }else{
        token.lexema[pointer++] = lookahead;
        switch (lookahead) {
            case S_MULTIPLICACAO:
                token.symbol = MULTIPLICACAO;
                break;
            case S_SOMA:
                token.symbol = SOMA;
                break;
            case S_SUBTRACAO:
                token.symbol = SUBTRACAO;
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
                }else{
                    token.symbol = 1000;
                }
        }
    }
    token.lexema[pointer] = '\0';
    return token;
}

void printToken(__TOKEN token){
    int i;
    if (token.symbol > 28) {
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