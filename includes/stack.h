#ifndef STACK_H_
#define STACK_H_

#include "globals.h"



__STACK * stack_create();
void stack_push(__STACK * , __TOKEN , int);
__TOKEN * stack_pop(__STACK * );
boolean stack_isEmpty(__STACK * );
void stack_node_free(__TOKEN ** );
void stack_free(__STACK ** );
void stack_free_scope(__STACK ** stack );
__TOKEN * stack_alloc_token(__TOKEN );
__TOKEN * stack_consult_top(__STACK * );
boolean stackVerifyExistsScope(__STACK * , __TOKEN );
boolean stackVerifyExistsAll(__STACK * , __TOKEN );
__SEMANTIC * stackConsultCurrentScope(__STACK * , __TOKEN );
__SEMANTIC * stackConsultAll(__STACK * , __TOKEN );

#endif
