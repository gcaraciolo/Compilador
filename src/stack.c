#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "symbols.h"

__STACK * stackCreate() {
	__STACK * stack = (__STACK *) malloc(sizeof(__STACK ));

	stack->top = NULL;

	return stack;
}

void stackPush(__STACK * stack, __TOKEN token, int type) {
	__LIST * _new = (__LIST *) malloc(sizeof(__LIST ));

	_new->token = stackAllocToken(token);
	_new->type = type;
	_new->scope = scope;
	_new->ant = stack->top;
	stack->top = _new;

}

__TOKEN * stackPop(__STACK * stack) {
	__TOKEN * token;
	__LIST * die = NULL;

	if (!stack->top) {
		token = NULL;
	} else {
		die = stack->top;
		token = die->token;
		stack->top = die->ant;
		free(die);
	}

	return token;
}

boolean stackIsEmpty(__STACK * stack) {

	return stack->top ? true : false;
}

void stackFreeNode(__TOKEN ** token) {

	free(*token);
	*token = NULL;
}

void stackFree(__STACK ** stack) {
	__LIST * die;

	while (*stack && (*stack)->top) {
		die = (*stack)->top;
		(*stack)->top = die->ant;
		stackFreeNode(&die->token);
		free(die);
	}

	free(*stack);
	*stack = NULL;
	stack = NULL;
}

void stackFreeScope(__STACK ** stack) {
	__LIST * die;

	while ((*stack)->top && (*stack)->top->scope == scope) {
		die = (*stack)->top;
		(*stack)->top = die->ant;
		stackFreeNode(&die->token);
		free(die);
	}
}

__TOKEN * stackAllocToken(__TOKEN token) {
	__TOKEN * point_token = (__TOKEN *) malloc(sizeof(__TOKEN ));

	point_token->symbol = token.symbol;
	strcpy(point_token->lexema, token.lexema);

	return point_token;
}

__TOKEN * stackConsultTop(__STACK * stack) {
	__LIST * value;

	if (stack->top) {
		value = stack->top;
	}

	return value->token;
}

boolean stackVerifyExistsScope(__STACK * stack, __TOKEN token) {
	__LIST * value = stack->top;

	while (value && value->scope == scope) {
		if (token.symbol == value->token->symbol
				&& strcmp(token.lexema, value->token->lexema) == 0) {
			return true;
		}
		value = value->ant;
	}
	return false;
}

boolean stackVerifyExistsAll(__STACK * stack, __TOKEN token) {
	__LIST * value = stack->top;

	while (value) {
		if (token.symbol == value->token->symbol
				&& strcmp(token.lexema, value->token->lexema) == 0) {
			return true;
		}
		value = value->ant;
	}
	return false;
}

/**
 * @return the symbol in the current scope or non declared symbol.
 */
__TYPE_EXPRESSION stackConsultCurrentScope(__STACK * stack, __TOKEN token) {
	__LIST * value = stack->top;
	__TYPE_EXPRESSION semantic;
	semantic.type = NON_DECLARED;

	while (value && value->scope == scope) {
		if (token.symbol == value->token->symbol
				&& strcmp(token.lexema, value->token->lexema) == 0) {
			semantic.type = value->type;
			break;
		}
		value = value->ant;
	}
	return semantic;
}

/**
 *
 * @return the symbol in the first scope localized or non declared symbol.
 */
__TYPE_EXPRESSION stackConsultAll(__STACK * stack, __TOKEN token) {
	__LIST * value = stack->top;
	__TYPE_EXPRESSION type_expression;
	type_expression.type = NON_DECLARED;

	while (value) {
		if (token.symbol == value->token->symbol
				&& strcmp(token.lexema, value->token->lexema) == 0) {
			type_expression.type = value->type;
            strcpy(type_expression.expr, token.lexema);
			break;
		}
		value = value->ant;
	}

	return type_expression;

}

