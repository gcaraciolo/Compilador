#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "symbols.h"
#include "errors.h"
#include "stack.h"

void parser() {
    labelCont = 1;
    varTempCont = 0;
	token = _SCAN();
	scope = -1;
	programa();
}

void programa() {

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

	symbols_table = stackCreate();
	bloco();
	stackFree(&symbols_table);

	if (END_OF_FILE != token.symbol) {
		errorMessage("programa so deve conter a funcao main");
	}

}

void bloco() {
	scope++;

	if (ABRE_CHAVES == token.symbol) {
		token = _SCAN();
	} else {
		errorMessage("esperado '{'");
	}

	decl_var(&symbols_table);
	while (isFirstCommand()) {
		comando();
	}

	if (FECHA_CHAVES == token.symbol) {
		token = _SCAN();
	} else {
		errorMessage("esperado '}'");
	}

	stackFreeScope(&symbols_table);
	scope--;

}

void multVariables(__STACK ** table, int type) {
	char msg[MAX_CHARACTER + 100];

	if (ID == token.symbol) {
		if (!stackVerifyExistsScope(*table, token)) {
			stackPush(*table, token, type);
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
					if (!stackVerifyExistsScope(*table, token)) {
						stackPush(*table, token, type);
					} else {
						strcpy(msg, "multiplas definicoes para \'");
						strcat(msg, token.lexema);
						strcat(msg, "\'");
						errorMessage(msg);
					}
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

void decl_var(__STACK ** table) {
	int type;

	do {
		type = getType();

		if (UNKNOW_TYPE != type) {
			token = _SCAN();
			multVariables(table, type);
		}

	} while (UNKNOW_TYPE != type);

}

void atribuicao() {
	__TYPE_EXPRESSION LValue, RValue;
    char instrucao[MAX_CHARACTER];
    instrucao[0] = '\0';
    
	if (ID == token.symbol) {		
		LValue = stackConsultAll(symbols_table, token);
		if(NON_DECLARED == LValue.type){
			errorMessage("variavel nao declarada");
		}
        strcat(instrucao, token.lexema);
        strcat(instrucao, " = ");
		token = _SCAN();
        
		if (IGUAL_ATRIBUICAO == token.symbol) {
			token = _SCAN();
			RValue = expressao();
			checkSemantic(LValue, RValue);
            strcat(instrucao, RValue.expr);
            
          //  writeCodeIntermindate(RValue);
            
			if (PONTO_VIRGULA == token.symbol) {
				token = _SCAN();
			} else {
				errorMessage("esperado `;'");
			}
		}
	}
    printf("%s\n", instrucao);
}

void comando_basico() {
	if (ID == token.symbol) {
		atribuicao();
	} else if (ABRE_CHAVES == token.symbol) {
		bloco();
	}
}

void exprRelacional() {
	__TYPE_EXPRESSION type_expression, left, right;
    char op[MAX_CHARACTER];
    op[0] = '\0';

	left = expressao();
	if (isExpressaoRelacional()) {
        strcat(op, token.lexema);
		token = _SCAN();
		right = expressao();
	} else {
		errorMessage("esperado uma expressao relacional");
	}

    type_expression = majorType(left, right);
    strcpy(type_expression.expr, left.expr);
    strcat(type_expression.expr, " ");
    strcat(type_expression.expr, op);
    strcat(type_expression.expr, " ");
    strcat(type_expression.expr, right.expr);
    arrayPush(type_expression.expr, " = ");
    newTemp();
    arrayPush(type_expression.expr, varTemp);
    printf("%s\n", type_expression.expr);
    
    
    
}

/**
 * expressao nao pode ser vazia.
 */
__TYPE_EXPRESSION expressao() {
	__TYPE_EXPRESSION type_termo, type_expressao_linha, type_expression;
    char instruct[MAX_CHARACTER];
    instruct[0] = '\0';
    
    type_termo = termo();
    type_expressao_linha = expressao_linha();
	type_expression = majorType(type_termo, type_expressao_linha);
    
	if (EMPTY == type_expression.type) {
		errorMessage("esperado uma expressao");
	}
    if (EMPTY != type_expressao_linha.type) {
        newTemp();
        strcat(instruct, varTemp);
        strcat(instruct, " = ");
        strcat(instruct, type_termo.expr);
        strcat(instruct, type_expressao_linha.expr);
        printf("%s\n", instruct);
        strcpy(type_expression.expr, varTemp);
    }
    
	return type_expression;
}

__TYPE_EXPRESSION expressao_linha() {
	__TYPE_EXPRESSION type_expression, type_termo, type_expressao_linha;
    char op[MAX_CHARACTER];
    op[0] = '\0';
    
    type_expression.type = EMPTY;
    type_expression.expr[0] = '\0';
    
	if (SOMA == token.symbol || SUBTRACAO == token.symbol) {
        strcpy(op, token.lexema);
		token = _SCAN();
        type_termo = termo();
        type_expressao_linha = expressao_linha();
		type_expression = majorType(type_termo, type_expressao_linha);
        
        if (EMPTY == type_expressao_linha.type) {
            arrayPush(type_termo.expr, " ");
            arrayPush(type_termo.expr, op);
            arrayPush(type_termo.expr, " ");
            type_expression = type_termo;
        } else {
            arrayPush(type_expressao_linha.expr, type_termo.expr);
            type_expression = type_expressao_linha;
            newTemp();
            arrayPush(type_expression.expr, " = ");
            arrayPush(type_expression.expr, varTemp);
            writeCodeIntermindate(type_expression);
            type_expression.expr[0] = ' ';
            type_expression.expr[1] = '\0';
            strcat(type_expression.expr, op);
            strcat(type_expression.expr, " ");
            strcat(type_expression.expr, varTemp);
            
        }
	}
    
	return type_expression;
}

__TYPE_EXPRESSION termo() {
    __TYPE_EXPRESSION type_fator, type_termo_linha, type_expression;
    char instruct[MAX_CHARACTER];
    instruct[0] = '\0';
    
    type_fator = fator();
    type_termo_linha = termo_linha();
    type_expression = majorType(type_fator, type_termo_linha);
    
    if (EMPTY == type_expression.type) {
        errorMessage("esperado um identificador");
    }
    if (EMPTY != type_termo_linha.type) {
        newTemp();
        strcat(instruct, varTemp);
        strcat(instruct, " = ");
        strcat(instruct, type_fator.expr);
        strcat(instruct, type_termo_linha.expr);
        printf("%s\n", instruct);
        strcpy(type_expression.expr, varTemp);
    }
    
    
    return type_expression;
}

__TYPE_EXPRESSION termo_linha() {
	__TYPE_EXPRESSION type_expression, type_fator, type_termo_linha;
    char op[4];
    op[0] = '\0';
    
    type_expression.type = EMPTY;
    type_expression.expr[0] = '\0';
    
	if (MULTIPLICACAO == token.symbol || DIVISAO == token.symbol) {
        strcpy(op, token.lexema);
		token = _SCAN();
        type_fator = fator();
        type_termo_linha = termo_linha();
		type_expression =  majorType(type_fator, type_termo_linha);
        
        if (EMPTY == type_termo_linha.type) {
            arrayPush(type_fator.expr, " ");
            arrayPush(type_fator.expr, op);
            arrayPush(type_fator.expr, " ");
            type_expression = type_fator;

        } else {
            arrayPush(type_termo_linha.expr, type_fator.expr);
            type_expression = type_termo_linha;
            newTemp();
            arrayPush(type_expression.expr, " = ");
            arrayPush(type_expression.expr, varTemp);
            writeCodeIntermindate(type_expression);
            type_expression.expr[0] = ' ';
            type_expression.expr[1] = '\0';
            strcat(type_expression.expr, op);
            strcat(type_expression.expr, " ");
            strcat(type_expression.expr, varTemp);

        }
        
	}
    
	return type_expression;
}

/**
 * @return token com a informação do tipo.
 */
__TYPE_EXPRESSION fator() {
	__TYPE_EXPRESSION type_expression;
    type_expression.expr[0] = '\0';
	switch (token.symbol) {
	case ID:
		// need to verify if id exists in symbols table and get the type of it
		type_expression = stackConsultAll(symbols_table, token);
		if(NON_DECLARED == type_expression.type){
			errorMessage("variavel nao declarada");
		}
		token = _SCAN();
		break;
	case DIGITO:
        strcat(type_expression.expr, token.lexema);
		type_expression.type = DIGITO;
		token = _SCAN();
		break;
	case DIGITO_FLUTUANTE:
        strcat(type_expression.expr, token.lexema);
		type_expression.type = DIGITO_FLUTUANTE;
		token = _SCAN();
		break;
	case LETRA:
        strcat(type_expression.expr, token.lexema);
		type_expression.type = LETRA;
		token = _SCAN();
		break;
	case ABRE_PARENTESES:
		token = _SCAN();
		type_expression = expressao();
		if (FECHA_PARENTESES == token.symbol) {
			token = _SCAN();
		} else {
			errorMessage("esperado `)'");
		}
		break;
	default:
		errorMessage("esperado um identificador");
		break;
	}
	return type_expression;
}

void iteracao() {
    char labelDoWhile[100], labelWhile[100], labelFimWhile[100];
    labelDoWhile[0] = '\0';
    labelWhile[0] = '\0';
    labelFimWhile[0] = '\0';
    
	switch (token.symbol) {
	case WHILE:
        newLabel();
        strcat(labelWhile, label);
        printf("%s:\n", labelWhile);
		token = _SCAN();
		if (ABRE_PARENTESES == token.symbol) {
			token = _SCAN();
			exprRelacional();
			if (FECHA_PARENTESES == token.symbol) {
				token = _SCAN();

                newLabel();
                strcat(labelFimWhile, label);
                printf("if %s == 0 goto %s\n", varTemp, label);
                
				if (isFirstCommand()) {
					comando();
                    printf("goto %s\n", labelWhile);
                    printf("%s:\n", labelFimWhile);
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
        newLabel();
        strcat(labelDoWhile, label);
        printf("%s:\n",labelDoWhile);
		token = _SCAN();
		if (isFirstCommand()) {
			comando();
		} else {
			errorMessage("esperado um comando");
		}
		if (WHILE == token.symbol) {
			token = _SCAN();
			if (ABRE_PARENTESES == token.symbol) {
				token = _SCAN();
				exprRelacional();
                
                printf("if %s != 0 goto %s\n",varTemp, labelDoWhile);
                
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

void condicional() {
    char instruction[MAX_CHARACTER], labelIf[100], labelFimIf[100];
    instruction[0] = '\0';
    labelIf[0]= '\0';
    labelFimIf[0] = '\0';
    
	switch (token.symbol) {
	case IF:
        strcat(instruction, token.lexema);
        strcat(instruction, " ");
		token = _SCAN();
		if (ABRE_PARENTESES == token.symbol) {
			token = _SCAN();
			exprRelacional();
            strcat(instruction, varTemp);
            strcat(instruction, " == 0 goto ");
            newLabel();
            strcat(labelIf, label);
            strcat(instruction, labelIf);
            printf("%s\n",instruction);
            
			if (FECHA_PARENTESES == token.symbol) {
				token = _SCAN();
				if (isFirstCommand()) {
					comando();
                    
                    newLabel();
                    printf("goto %s\n",label);
                    strcat(labelFimIf, label);
				} else {
					errorMessage("esperado um comando");
				}
                
                printf("%s:\n", labelIf);
				if (ELSE == token.symbol) {
					token = _SCAN();
					if (isFirstCommand()) {
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
    printf("%s:\n",labelFimIf);
}

void comando() {
    if (isFirstBasicCommand()) {
        comando_basico();
    } else if (isFirstInteration()){
        iteracao();
    } else if (isFirstConditional()){
        condicional();
    } 
}

int getType() {
	switch (token.symbol) {
	case INT:
		return DIGITO;
	case FLOAT:
		return DIGITO_FLUTUANTE;
	case CHAR:
		return LETRA;
	}
	return UNKNOW_TYPE;
}

boolean isFirstBasicCommand() {
    switch (token.symbol) {
        case ID:
        case ABRE_CHAVES:
            return true;
    }
    return false;
}

boolean isFirstDeclaration() {
	switch (token.symbol) {
	case INT:
	case FLOAT:
	case CHAR:
		return true;
	}
	return false;
}

boolean isExpressaoRelacional() {
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

boolean isFirstCommand() {
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

boolean isOparitimetico() {
	switch (token.symbol) {
	case SOMA:
	case SUBTRACAO:
	case MULTIPLICACAO:
	case DIVISAO:
	case IGUAL_ATRIBUICAO:
		return true;
	}
	return false;
}

boolean isFirstBlock() {
	switch (token.symbol) {
	case ABRE_CHAVES:
		return true;
	}
	return false;
}

boolean isFirstAssing() {
	switch (token.symbol) {
	case ID:
		return true;
	}
	return false;
}

boolean isFirstInteration() {
	switch (token.symbol) {
	case DO:
	case WHILE:
		return true;
	}
	return false;
}

boolean isFirstConditional() {
    switch (token.symbol) {
        case IF:
            return true;
    }
    return false;
}

void checkSemantic(__TYPE_EXPRESSION LValue, __TYPE_EXPRESSION RValue) {
	boolean ok = true;
    
	if (DIGITO_FLUTUANTE == LValue.type && DIGITO == RValue.type) {
		ok = true;
	} else if (DIGITO == LValue.type && DIGITO_FLUTUANTE == RValue.type) {
		ok = false;
	}else if (RValue.type != LValue.type) {
		ok = false;
	}

	if(!ok){
		errorMessage("tipos incompativeis");
	}
}

__TYPE_EXPRESSION majorType(__TYPE_EXPRESSION type0, __TYPE_EXPRESSION type1) {
	__TYPE_EXPRESSION type_expression;
    
    if (type0.type == type1.type) {
        type_expression.type = type0.type;
    } else if (EMPTY == type0.type && EMPTY == type1.type) {
        type_expression.type = EMPTY;
        type_expression.expr[0] = '\0';
    } else if (EMPTY == type0.type) {
        type_expression = type1;
    } else if (EMPTY == type1.type) {
        type_expression = type0;
    } else if ((DIGITO == type0.type && DIGITO_FLUTUANTE == type1.type)
               || (DIGITO_FLUTUANTE == type0.type && DIGITO == type1.type)) {
        type_expression.type = DIGITO_FLUTUANTE;
    } else {
        errorMessage("tipos incompativeis");
    }
	return type_expression;
}

void newTemp() {
    char tempAux[MAX_CHARACTER];
    char number[100];
    sprintf(number, "%d", varTempCont);
    strcpy(tempAux, "t");
    strcat(tempAux, number);
    strcpy(varTemp, tempAux);
    varTempCont++;
}

void getLabel(){
    printf("L%d ", labelCont);
}

void newLabel() {
    char tempAux[MAX_CHARACTER];
    char number[100];
    sprintf(number, "%d", labelCont);
    strcpy(tempAux, "L");
    strcat(tempAux, number);
    strcpy(label, tempAux);
    labelCont++;
}

void writeCodeIntermindate(__TYPE_EXPRESSION type_expression) {
    if (EMPTY != type_expression.type) {
        printf("%s\n", type_expression.expr);
    }
}

void arrayPush(char * array0, const char * array1) {
    char new_array[MAX_CHARACTER];
    new_array[0] = '\0';
   
    strcat(new_array, array1);
    strcat(new_array, array0);
    strcpy(array0, new_array);
}











