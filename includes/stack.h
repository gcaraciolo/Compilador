#ifndef STACK_H_
#define STACK_H_

#include "globals.h"



__STACK * stackCreate();
void stackPush(__STACK * , __TOKEN , int);
__TOKEN * stackPop(__STACK * );
boolean stackIsEmpty(__STACK * );
void stackFreeNode(__TOKEN ** );
void stackFree(__STACK ** );
void stackFreeScope(__STACK ** stack );
__TOKEN * stackAllocToken(__TOKEN );
__TOKEN * stackConsultTop(__STACK * );
boolean stackVerifyExistsScope(__STACK * , __TOKEN );
boolean stackVerifyExistsAll(__STACK * , __TOKEN );
__TYPE_EXPRESSION stackConsultCurrentScope(__STACK * , __TOKEN );
__TYPE_EXPRESSION stackConsultAll(__STACK * , __TOKEN );

#endif
