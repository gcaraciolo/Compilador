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

__TOKEN token;

void parser();

void decl_var();
int tipo();

void programa();
void bloco();
void comando();
void comando_basico();
void iteracao();
void atribuicao();
void expr_relacional();
void expr_arit();
void termo();
void fator();

#endif