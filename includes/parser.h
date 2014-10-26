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
void programa ();
void bloco();
void mult_variables(__STACK ** , int );
void decl_var(__STACK ** );
void atribuicao();
void comando_basico();
void expr_relacional();
__TOKEN expressao();
__TOKEN expressao_linha();
__TOKEN termo();
__TOKEN termo_linha();
__TOKEN fator();
void iteracao();
void condicional();
void comando();
boolean isExpressaoRelacional();
int getType();
boolean isFirstBasicCommand();
boolean isFirstDeclaration();
boolean isExpressaoRelacional();
boolean isFirstCommand();
boolean isOparitimetico();
boolean isFirstBlock();
boolean isFirstAssing();
boolean isFirstInteration();
boolean isFirstConditional();
void checkSemantic(__SEMANTIC, __SEMANTIC );
__SEMANTIC majorType(__SEMANTIC , __SEMANTIC );


#endif
