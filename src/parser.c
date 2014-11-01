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

void mult_variables(__STACK ** table, int type) {
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
			mult_variables(table, type);
		}

	} while (UNKNOW_TYPE != type);

}

void atribuicao() {
	__TYPE_EXPRESSION LValue, RValue;
	if (ID == token.symbol) {		
		LValue = stackConsultAll(symbols_table, token);
		if(NON_DECLARED == LValue.type){
			errorMessage("variavel nao declarada");
		}
		token = _SCAN();
		if (IGUAL_ATRIBUICAO == token.symbol) {
			token = _SCAN();
			RValue = expressao();
			checkSemantic(LValue, RValue);
			if (PONTO_VIRGULA == token.symbol) {
				token = _SCAN();
			} else {
				errorMessage("esperado `;'");
			}
		}
	}
}

void comando_basico() {
	if (ID == token.symbol) {
		atribuicao();
	} else if (ABRE_CHAVES == token.symbol) {
		bloco();
	}
}

void expr_relacional() {
	__TYPE_EXPRESSION left, right;
	left = expressao();
	if (isExpressaoRelacional()) {
		token = _SCAN();
		right = expressao();
	} else {
		errorMessage("esperado uma expressao relacional");
	}
}

/**
 * expressao nao pode ser vazia.
 */
__TYPE_EXPRESSION expressao() {
	__TYPE_EXPRESSION type_termo, type_expressao_linha, type_expression;
    
    type_termo = termo();
    type_expressao_linha = expressao_linha();
	type_expression = majorType(type_termo, type_expressao_linha);
    
	if (EMPTY == type_expression.type) {
		errorMessage("esperado uma expressao");
	}
	return type_expression;
}

__TYPE_EXPRESSION expressao_linha() {
	__TYPE_EXPRESSION type_expression;
    type_expression.type = EMPTY;
	if (SOMA == token.symbol || SUBTRACAO == token.symbol) {
		token = _SCAN();
		type_expression = majorType(termo(), expressao_linha());
	}
	return type_expression;
}

__TYPE_EXPRESSION termo() {
	return majorType(fator(), termo_linha());
}

__TYPE_EXPRESSION termo_linha() {
	__TYPE_EXPRESSION type_expression;
    type_expression.type = EMPTY;
	if (MULTIPLICACAO == token.symbol || DIVISAO == token.symbol) {
		token = _SCAN();
		type_expression =  majorType(fator(), termo_linha());
	}
	return type_expression;
}

/**
 * @return token com a informação do tipo.
 */
__TYPE_EXPRESSION fator() {
	__TYPE_EXPRESSION type_expression;
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
		type_expression.type = DIGITO;
		token = _SCAN();
		break;
	case DIGITO_FLUTUANTE:
		type_expression.type = DIGITO_FLUTUANTE;
		token = _SCAN();
		break;
	case LETRA:
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
	switch (token.symbol) {
	case WHILE:
		token = _SCAN();
		if (ABRE_PARENTESES == token.symbol) {
			token = _SCAN();
			expr_relacional();
			if (FECHA_PARENTESES == token.symbol) {
				token = _SCAN();
				if (isFirstCommand()) {
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
		if (isFirstCommand()) {
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

void condicional() {
	switch (token.symbol) {
	case IF:
		token = _SCAN();
		if (ABRE_PARENTESES == token.symbol) {
			token = _SCAN();
			expr_relacional();
			if (FECHA_PARENTESES == token.symbol) {
				token = _SCAN();
				if (isFirstCommand()) {
					comando();
				} else {
					errorMessage("esperado um comando");
				}
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

