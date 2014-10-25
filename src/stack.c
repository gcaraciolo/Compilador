#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

__STACK * stack_create(){
    __STACK * stack = (__STACK*) malloc(sizeof(__STACK));
    
    stack->topo = NULL;
    
    return stack;
}

void stack_push(__STACK * stack, __TOKEN token, int type, int scope){
    __LIST * _new = (__LIST*) malloc(sizeof(__LIST));
    
    _new->token     =   stack_alloc_token(token);
    _new->type      =   type;
    _new->scope     =   scope;
    _new->ant       =   stack->topo;
    stack->topo     =   _new;
    
}

__TOKEN * stack_pop(__STACK * stack){
    __TOKEN *token;
    __LIST *die = NULL;
    
    if(!stack->topo){
        token = NULL;
    }else{
        die             =   stack->topo;
        token           =   die->token;
        stack->topo     =   die->ant;
        free(die);
    }
    
    return token;
}

__TOKEN * stack_consult(__STACK * stack){
    __LIST * value;
    
    if(stack->topo){
        value = stack->topo;
    }
    
    return value->token;
}

boolean stack_isEmpty(__STACK * stack){
    
    return stack->topo ? true : false;
}

void stack_node_free(__TOKEN ** token){
    
    free(* token);
    *token = NULL;
}

void stack_free(__STACK ** stack){
    __LIST * die;
    
    while((*stack)->topo){
        die             =   (*stack)->topo;
        (*stack)->topo  =   die->ant;
        stack_node_free(&die->token);
        free(die);	
    }
    
    free(*stack);
}

__TOKEN * stack_alloc_token(__TOKEN token){
    __TOKEN * point_token = (__TOKEN*) malloc(sizeof(__TOKEN));
    
    point_token->symbol = token.symbol;
    strcpy(point_token->lexema, token.lexema);
    
    return point_token;
}