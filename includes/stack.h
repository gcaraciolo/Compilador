#ifndef STACK_H_
#define STACK_H_

#include "globals.h"


__STACK * stack_create();
void stack_push(__STACK * stack, __TOKEN token, int type, int scope);
__TOKEN * stack_pop(__STACK * stack);
boolean stack_isEmpty(__STACK * stack);
void stack_node_free(__TOKEN ** token);
void stack_free(__STACK ** stack);
void stack_free_scope(__STACK ** stack, int scope);
__TOKEN * stack_alloc_token(__TOKEN token);
__TOKEN * stack_consult_top(__STACK * stack);
boolean stack_consult_scope(__STACK * stack, int scope, __TOKEN token);
boolean stack_consult_all(__STACK * stack, __TOKEN token);


#endif
