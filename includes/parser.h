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
void multVariables(__STACK ** , int );
void decl_var(__STACK ** );
void atribuicao();
void comando_basico();
void exprRelacional();
__TYPE_EXPRESSION expressao();
__TYPE_EXPRESSION expressao_linha();
__TYPE_EXPRESSION termo();
__TYPE_EXPRESSION termo_linha();
__TYPE_EXPRESSION fator();
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
void checkSemantic(__TYPE_EXPRESSION, __TYPE_EXPRESSION );
__TYPE_EXPRESSION majorType(__TYPE_EXPRESSION , __TYPE_EXPRESSION );
void getTemp();
void newTemp();
void getLabel();
void newLabel();
void writeCodeIntermindate(__TYPE_EXPRESSION );
void arrayPush(char * , const char * );


#endif
