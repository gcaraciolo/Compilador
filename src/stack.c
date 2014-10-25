#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

__STACK * stack_create(){
    __STACK * stack = (__STACK*) malloc(sizeof(__STACK));
    
    stack->top = NULL;
    
    return stack;
}

void stack_push(__STACK * stack, __TOKEN token, int type, int scope){
    __LIST * _new = (__LIST*) malloc(sizeof(__LIST));
    
    _new->token     =   stack_alloc_token(token);
    _new->type      =   type;
    _new->scope     =   scope;
    _new->ant       =   stack->top;
    stack->top     =   _new;
    
}

__TOKEN * stack_pop(__STACK * stack){
    __TOKEN *token;
    __LIST *die = NULL;
    
    if(!stack->top){
        token = NULL;
    }else{
        die             =   stack->top;
        token           =   die->token;
        stack->top     =   die->ant;
        free(die);
    }
    
    return token;
}

boolean stack_isEmpty(__STACK * stack){
    
    return stack->top ? true : false;
}

void stack_node_free(__TOKEN ** token){
    
    free(* token);
    *token = NULL;
}

void stack_free(__STACK ** stack){
    __LIST * die;
    
    while(*stack && (*stack)->top){
        die             =   (*stack)->top;
        (*stack)->top   =   die->ant;
        stack_node_free(&die->token);
        free(die);
    }
    
    free(*stack);
    *stack = NULL;
    stack = NULL;
}

void stack_free_scope(__STACK ** stack, int scope){
    __LIST * die;
    
    while((*stack)->top && (*stack)->top->scope == scope){
        die             =   (*stack)->top;
        (*stack)->top   =   die->ant;
        stack_node_free(&die->token);
        free(die);
    }
}

__TOKEN * stack_alloc_token(__TOKEN token){
    __TOKEN * point_token = (__TOKEN*) malloc(sizeof(__TOKEN));
    
    point_token->symbol = token.symbol;
    strcpy(point_token->lexema, token.lexema);
    
    return point_token;
}

__TOKEN * stack_consult_top(__STACK * stack){
    __LIST * value;
    
    if(stack->top){
        value = stack->top;
    }
    
    return value->token;
}



boolean stack_consult_scope(__STACK * stack, int scope, __TOKEN token){
    __LIST * value = stack->top;
    
    while (value && value->scope == scope) {
        if (token.symbol == value->token->symbol && strcmp(token.lexema, value->token->lexema) == 0) {
            return true;
        }
        value = value->ant;
    }
    return false;
}



boolean stack_consult_all(__STACK * stack, __TOKEN token){
    __LIST * value = stack->top;
    
    while (value) {
        if (token.symbol == value->token->symbol && strcmp(token.lexema, value->token->lexema) == 0) {
            return true;
        }
        value = value->ant;
    }
    return false;
}













