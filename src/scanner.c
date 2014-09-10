#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "symbols.h"
#include "globals.h"
#include "messages.h"
#include "fileActions.h"

boolean verifyLookahead(char lookahead){
    switch (lookahead) {
        case SPACE:
        case NEW_LINE:
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

void errorMessage(__TOKEN token, const char *message){
    printf("ERRO na linha %d, coluna %d, ultimo token lido %s: %s\n", linha, coluna, token.lexema, message);
}

boolean verifyToken(__TOKEN token){
    if(ERROR_OPERADOR_MAL_FORMADO == token.symbol){
        errorMessage(token, "operador != mal formado");
        return false;
    } else if (ERROR_CHAR_MAL_FORMADO == token.symbol){
        errorMessage(token, "caracter mal formado");
        return false;
    } else if (ERROR_FLOAT_MAL_FORMADO == token.symbol){
        errorMessage(token, "float mal formado");
        return false;
    } else if (ERROR_UNKNOW_SYMBOL == token.symbol){
        errorMessage(token, "caracter invalido");
        return false;
    } else if (ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO == token.symbol){
        errorMessage(token, "comentario nao encerrado.");
        return false;
    }
    return true;
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

int commentLogic(char *lookahead, __TOKEN *token){
    char pointerFile;
    pointerFile = readCharacter();
    if (S_DIVISAO == pointerFile) {
        /* inline comment */
        while (NEW_LINE != pointerFile && EOF != pointerFile) {
            pointerFile = readCharacter();
        }
        //if it's EOF, there's no diference about read again (the symbol its equal in return of readCharacter)
        *lookahead = readCharacter();
        return COMMENT;
    } else if (S_MULTIPLICACAO == pointerFile){
        while (true) {
            pointerFile = readCharacter();
            if (S_MULTIPLICACAO == pointerFile) {
                pointerFile = readCharacter();
                if (S_DIVISAO == pointerFile) {
                    break;
                } else if(EOF == pointerFile){
                    return ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO;
                }
            } else if(EOF == pointerFile){
                return ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO;
            }
        }
        *lookahead = readCharacter();
        return COMMENT;
    }
    (*token).lexema[0] = *lookahead;
    (*token).symbol = DIVISAO;
    *lookahead = pointerFile;
    return DIVISAO;
}

__TOKEN _SCAN(){
	static char lookahead = SPACE;
    static __TOKEN token;
    long pointer;
    int statusComment = -1;

    start:
    pointer = 0;
    //incluir comment nesse while
    while (isspace(lookahead)) {
        lookahead = readCharacter();
    }

    if (EOF == lookahead) {
        token.lexema[pointer++] = lookahead;
        token.symbol = END_OF_FILE;
    } else if(S_DIVISAO == lookahead){
        statusComment = commentLogic(&lookahead, &token);
        if (COMMENT == statusComment) {
            goto start;
        } else if(ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO == statusComment) {
            pointer = strlen(token.lexema);
            token.symbol = ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO;
        } else if (DIVISAO == statusComment) {
          //token already filled, just update pointer for put '\0' in the correct position in the end of this func
          pointer++;
        }
    } else {
        token.lexema[pointer++] = lookahead;
        switch (lookahead) {
            /* oparitimeticos */
            case S_SOMA:
                token.symbol = SOMA;
                lookahead = readCharacter();
                break;
            case S_SUBTRACAO:
                token.symbol = SUBTRACAO;
                lookahead = readCharacter();
                break;
            case S_IGUAL:
                lookahead = readCharacter();
                if(S_IGUAL == lookahead){
                    token.symbol = IGUAL_COMPARACAO;
                    token.lexema[pointer++] = lookahead;
                    lookahead = readCharacter();
                }else{
                    token.symbol = IGUAL_ATRIBUICAO;
                }
                break;
            case S_MULTIPLICACAO:
                token.symbol = MULTIPLICACAO;
                lookahead = readCharacter();
                break;
            case S_MAIOR:
                lookahead = readCharacter();
                if (S_IGUAL == lookahead) {
                    token.lexema[pointer++] = lookahead;
                    token.symbol = MAIOR_IGUAL;
                    lookahead = readCharacter();
                } else {
                    token.symbol = MAIOR;
                }
                break;
            case S_MENOR:
                lookahead = readCharacter();
                if (S_IGUAL == lookahead) {
                    token.lexema[pointer++] = lookahead;
                    token.symbol = MAIOR_IGUAL;
                    lookahead = readCharacter();
                } else {
                    token.symbol = MENOR;
                }
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

            case S_ASPAS_SIMPLES:
                lookahead = readCharacter();
                token.lexema[pointer++] = lookahead;
                if (false != isalnum(lookahead)) {
                    lookahead = readCharacter();
                    token.lexema[pointer++] = lookahead;
                    if (S_ASPAS_SIMPLES == lookahead) {
                        token.symbol = CHAR;
                        lookahead = readCharacter();
                    } else {
                        token.symbol = ERROR_CHAR_MAL_FORMADO;
                    }
                } else {
                    token.symbol = ERROR_CHAR_MAL_FORMADO;
                }
                break;
            /* especiais */
            case S_ABRE_CHAVES:
                token.symbol = ABRE_CHAVES;
                lookahead = readCharacter();
                break;
            case S_FECHA_CHAVES:
                token.symbol = FECHA_CHAVES;
                lookahead = readCharacter();
                break;
            case S_ABRE_PARENTESES:
                token.symbol = ABRE_PARENTESES;
                lookahead = readCharacter();
                break;
            case S_FECHA_PARENTESES:
                token.symbol = FECHA_PARENTESES;
                lookahead = readCharacter();
                break;
            case S_VIRGULA:
                token.symbol = VIRGULA;
                lookahead = readCharacter();
                break;
            case S_PONTO_VIRGULA:
                token.symbol = PONTO_VIRGULA;
                lookahead = readCharacter();
                break;
            case S_PONTO:
                /* could be float*/
                goto digito_float;

            default:
                if (false != isdigit(lookahead)) {
                    while (true) {
                        lookahead = readCharacter();
                        if (false != isdigit(lookahead)) {
                            token.lexema[pointer++] = lookahead;
                        } else if(S_PONTO == lookahead){
                            /* float */
                            token.lexema[pointer++] = lookahead;
                            digito_float:
                            lookahead = readCharacter();
				if (false != isdigit(lookahead)) {
                                token.symbol = DIGITO_FLUTUANTE;
                                token.lexema[pointer++] = lookahead;
                                while (isdigit(lookahead = readCharacter())) {
                                    token.lexema[pointer++] = lookahead;
                                }
                                break;
                            } else {
                                token.symbol = ERROR_FLOAT_MAL_FORMADO;
                                break;
                            }
                        } else {
                            token.symbol = DIGITO;
                            break;
                        }
                    }
                } else if (false != isalpha(lookahead) || S_UNDERLINE == lookahead){
                    /* identificador or keyword */
                    while (true) {
                        lookahead = readCharacter();
                        if(false != isalnum(lookahead) || S_UNDERLINE == lookahead){
                            token.lexema[pointer++] = lookahead;
                        }else{
                            break;
                        }
                    }
                    token.symbol = ID;
                } else{
                    token.symbol = ERROR_UNKNOW_SYMBOL;
                }
        }
    }
    token.lexema[pointer] = '\0';
    if (ID == token.symbol) {
        if(verifyKeyword(token.lexema)){
            token.symbol = verifyKeyword(token.lexema);
        }
    }
    if(!verifyToken(token)){
        exit(1);
    }

    return token;
}

void printToken(__TOKEN token){
    printf("|lexema, symbol|: |%s, %d|\n",token.lexema, token.symbol);
}

void readFile(){
    __TOKEN token;
    linha = 1;
    coluna = 0;
    while(true){
        token = _SCAN();
        if(END_OF_FILE == token.symbol){
            printf("Build succeeded.\n");
            break;
        }
        //printToken(token);
    }
}













