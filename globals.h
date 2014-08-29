#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>

typedef enum{
	false, true
}boolean;

FILE *file;

#define SPACE ' '
#define NEW_LINE '\n'
#define TAB_ '\t'

#define MAX_CHARACTER 257

typedef struct __TOKEN{
    char lexema[MAX_CHARACTER];
    int symbol;
}__TOKEN;



#endif