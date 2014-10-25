#ifndef STACK_H_
#define STACK_H_

#include "globals.h"

__STACK *   stack_create();
void        stack_push(__STACK * , __TOKEN, int, int );
__TOKEN *   stack_pop(__STACK * );
__TOKEN *   stack_consult(__STACK * );
boolean     stack_isEmpty(__STACK * );
void        stack_node_free(__TOKEN ** );
void        stack_free(__STACK ** );
__TOKEN *   stack_alloc_token(__TOKEN );
    
#endif
