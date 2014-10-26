//
//  globals.h
//  Compilador
//
//  Created by Guilherme Caraciolo on 14/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>

typedef enum{
	false, true
}boolean;

FILE *file;

#define MAX_CHARACTER 257

typedef struct __TOKEN __TOKEN;
typedef struct __SEMANTIC __SEMANTIC;
typedef struct __LIST __LIST;
typedef struct __STACK __STACK;


struct __TOKEN{
    char lexema[MAX_CHARACTER];
    int symbol;
};

struct __SEMANTIC{
    char expr[MAX_CHARACTER];
    int type;
};

struct __LIST {
    struct __LIST * ant;
    struct __TOKEN * token;
    int type;
    int scope; //scope of entire block
};

struct __STACK {
    __LIST * top;
};


int linha, coluna;
__TOKEN token;
int scope; // scope of blocks
__STACK * symbols_table;

#endif
