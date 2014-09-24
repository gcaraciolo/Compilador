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

boolean parser();
boolean programa ();
boolean bloco(boolean);
boolean mult_variables();
int getTipo();
boolean decl_var();
boolean expressao();
boolean atribuicao();
boolean comando_basico();
boolean isExpressaoRelacional();
boolean expr_relacional();
boolean termo();
boolean fator();
boolean iteracao();
boolean condicional();
boolean comando();

#endif
