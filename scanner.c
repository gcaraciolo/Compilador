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

int invalidCharacters(char *lookahead){
    char auxComment;
    char pointerFile = *lookahead;
    while (true) {
    start:
            pointerFile = readCharacter();
        if (isspace(pointerFile)) {
            continue;
        } else if (EOF == pointerFile) {
            //ending file without comment
            *lookahead = EOF;
            break;
        } else if (S_DIVISAO != pointerFile) {
            //valid token
            *lookahead = pointerFile;
            break;
        } else {
            //S_MULTIPLICACAO
        comment:
            auxComment = readCharacter();
            if (S_MULTIPLICACAO == auxComment) {
                /* block comment */
                while(true) {
                    auxComment = readCharacter();
                    if (S_MULTIPLICACAO == auxComment) {
                        auxComment = readCharacter();
                        if (S_DIVISAO == auxComment) {
                            //going to begin of this funcion
                            goto start;
                        }else if (EOF == auxComment){
                            //chamar erro e aborta programa
                            return ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO;
                        }
                    }else if (EOF == auxComment){
                        //chamar erro e aborta programa
                        return ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO;
                    }
                }
                
            } else if (S_DIVISAO == auxComment){
                /* inline comment */
                while (true) {
                    auxComment = readCharacter();
                    if (NEW_LINE == auxComment || NEW_LINE2 == auxComment || EOF == auxComment) {
                        break;
                    }
                }
                
            } else {
                //going to case: S_DIVISAO
                *lookahead = pointerFile;
                break;
            }
        }
    }
    return true;
}

__TOKEN _SCAN(){
	static char lookahead = SPACE;
    static __TOKEN token;
    int pointer = 0;
    
    //incluir comment nesse while
    //while (isspace(lookahead)) {
      //  lookahead = readCharacter();
    //}
    
    if(ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO == invalidCharacters(&lookahead)){
        pointer += strlen(token.lexema);
        token.symbol = ERROR_TERMINAR_ARQUIVO_SEM_FECHAR_COMENTARIO;
    }else if (EOF == lookahead) {
        token.lexema[pointer++] = lookahead;
        token.symbol = END_OF_FILE;
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
            case S_DIVISAO:
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
                if (true == isalnum(lookahead)) {
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
                lookahead = readCharacter();
                goto digito_float;
                
            default:
                if (true == isdigit(lookahead)) {
                    while (true) {
                        lookahead = readCharacter();
                        if (true == isdigit(lookahead)) {
                            token.lexema[pointer++] = lookahead;
                        } else if(S_PONTO == lookahead){
                            /* float */
                            digito_float:
                            token.lexema[pointer++] = lookahead;
                            lookahead = readCharacter();
                            if (true == isdigit(lookahead)) {
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
    printf("lexema: %s\n",token.lexema);
    printf("symbol: %d\n\n",token.symbol);
    
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

void readFile(){
    __TOKEN token;
    linha = 1;
    coluna = 0;
    while(true){
        token = _SCAN();
        if(token.symbol == END_OF_FILE){
            printf("Build succeeded.\n");
            break;
        }
        printToken(token);
        if(!verifyToken(token)){
            break;
        }
    }
}













