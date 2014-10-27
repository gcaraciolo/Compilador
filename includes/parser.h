//
//  parser.h
//  Compilador
//
//  Created by Guilherme Caraciolo on 14/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#ifndef Compilador_parser_h
#define Compilador_parser_h

#include "globals.h"

void parser();
void programa();
void bloco(boolean);
void mult_variables(__STACK **, int);
int getType();
void decl_var(__STACK ** );
void expressao();
void atribuicao();
void comando_basico();
boolean isExpressaoRelacional();
boolean isCommandFirst();
void expr_relacional();
void termo();
void fator();
void iteracao();
void condicional();
void comando();

#endif
