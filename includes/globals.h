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

typedef struct __TOKEN{
    char lexema[MAX_CHARACTER];
    int symbol;
}__TOKEN;

int linha, coluna;
__TOKEN token;

#endif