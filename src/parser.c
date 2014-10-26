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

	symbols_table = stack_create();
	bloco();
	stack_free(&symbols_table);

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

	stack_free_scope(&symbols_table, scope);
	scope--;

}

void mult_variables(__STACK ** table, int type) {
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
	__TOKEN LValue, RValue;
	if (ID == token.symbol) {
		LValue = token = _SCAN();
		if (IGUAL_ATRIBUICAO == token.symbol) {
			token = _SCAN();
			RValue = expressao();
			if (PONTO_VIRGULA == token.symbol) {
				token = _SCAN();
			} else {
				errorMessage("esperado `;'");
			}
		}
	}
	checkSemantic(LValue, RValue);
}

void comando_basico() {
	if (ID == token.symbol) {
		atribuicao();
	} else if (ABRE_CHAVES == token.symbol) {
		bloco();
	}
}

void expr_relacional() {
	__TOKEN type0, type1;
	type0 = expressao();
	if (isExpressaoRelacional()) {
		token = _SCAN();
		type1 = expressao();
	} else {
		errorMessage("esperado uma expressao relacional");
	}
	checkSemantic(type0, type1);
}

/**
 * expressao nao pode ser vazia.
 */
__TOKEN expressao() {
	__TOKEN type;
	type = majorType(termo(), expressao_linha());
	if (EMPTY == type.symbol) {
		errorMessage("esperado uma expressao"); //program is aborted;
	}
	return type;
}

__TOKEN expressao_linha() {
	__TOKEN empty;
	if (SOMA == token.symbol || SUBTRACAO == token.symbol) {
		token = _SCAN();
		return majorType(termo(), expressao_linha());
	}
	empty.symbol = EMPTY;
	return empty;
}

__TOKEN termo() {
	return majorType(fator(), termo_linha());
}

__TOKEN termo_linha() {
	__TOKEN empty;
	if (MULTIPLICACAO == token.symbol || DIVISAO == token.symbol) {
		token = _SCAN();
		return majorType(fator(), termo_linha());
	}
	empty.symbol = EMPTY;
	return empty;
}

/**
 * @return token com a informação do tipo.
 */
__TOKEN fator() {
	__TOKEN type;
	switch (token.symbol) {
	case ID:
	case DIGITO:
	case DIGITO_FLUTUANTE:
	case LETRA:
		type = token = _SCAN();
		break;
	case ABRE_PARENTESES:
		token = _SCAN();
		type = expressao();
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
	return type;
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
		return INT;
	case FLOAT:
		return FLOAT;
	case CHAR:
		return CHAR;
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

void checkSemantic(__SEMANTIC type0, __SEMANTIC type1) {
	boolean ok = false;
    /*
	if (DIGITO_FLUTUANTE == type0.symbol && DIGITO == type1.symbol) {
		ok = true;
	} else if (DIGITO == type0.symbol && DIGITO_FLUTUANTE == type1.symbol) {
		ok = false;
	}else if (type0.symbol != type1.symbol) {
		ok = false;
	}

	if(!ok){
		errorMessage("tipos incompativeis");
	}*/
}

__SEMANTIC majorType(__SEMANTIC type0, __SEMANTIC type1) {
	__SEMANTIC type;
    /*
	if (type0.symbol == type1.symbol) {
		type = type0;
	} else if (EMPTY == type0.symbol && EMPTY == type1.symbol) {
		type = type0;
	} else if (EMPTY == type0.symbol) {
		type = type1;
	} else if (EMPTY == type1.symbol) {
		type = type0;
	} else {
		if (DIGITO_FLUTUANTE == type0.symbol && DIGITO == type1.symbol) {
			type = type0;
		} else if (DIGITO_FLUTUANTE == type1.symbol && DIGITO == type0.symbol) {
			type = type1;
		} else if (LETRA == type0.symbol && LETRA != type1.symbol) {
			errorMessage("tipos incompativeis");
		}
	}*/
	return type;
}

