//
//  errors.h
//  Compilador
//
//  Created by Guilherme Caraciolo on 15/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#ifndef Compilador_errors_h
#define Compilador_errors_h

#include "globals.h"

void errorMessage(__TOKEN, const char *);
void verifyToken(__TOKEN);

#endif
