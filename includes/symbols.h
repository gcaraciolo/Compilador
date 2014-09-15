//
//  symbols.h
//  Compilador
//
//  Created by Guilherme Caraciolo on 14/09/14.
//  Copyright (c) 2014 CARACIOLO. All rights reserved.
//

#ifndef SYMBOLS_H_
#define SYMBOLS_H_

/* reserved words */
#define MAIN                            1
#define IF                              2
#define ELSE                            3
#define WHILE                           4
#define DO                              5
#define FOR                             6
#define INT                             7
#define FLOAT                           8
#define CHAR                            9

/* oprelacional */
#define MENOR                           10
#define MAIOR                           11
#define MENOR_IGUAL                     12
#define MAIOR_IGUAL                     13
#define IGUAL_COMPARACAO                14
#define DIFERENTE_COMPARACAO            15

/* oparitimetico */
#define SOMA                            16
#define SUBTRACAO                       17
#define MULTIPLICACAO                   18
#define DIVISAO                         19
#define IGUAL_ATRIBUICAO                20

/* special characters */
#define ABRE_PARENTESES                 21
#define FECHA_PARENTESES                22
#define ABRE_CHAVES                     23
#define FECHA_CHAVES                    24
#define VIRGULA                         25
#define PONTO_VIRGULA                   26

/* letra */
#define LETRA                           27

/* digito */
#define DIGITO                          28
#define DIGITO_FLUTUANTE                29

/* identificador */
#define ID                              30

#define COMMENT                         31

/* symbols */
#define S_MENOR                         '<'
#define S_MAIOR                         '>'
#define S_IGUAL                         '='
#define S_MULTIPLICACAO                 '*'
#define S_DIVISAO                       '/'
#define S_SOMA                          '+'
#define S_SUBTRACAO                     '-'
#define S_ABRE_PARENTESES               '('
#define S_FECHA_PARENTESES              ')'
#define S_ABRE_CHAVES                   '{'
#define S_FECHA_CHAVES                  '}'
#define S_PONTO_VIRGULA                 ';'
#define S_VIRGULA                       ','
#define S_EXCLAMACAO                    '!'
#define S_UNDERLINE                     '_'
#define S_PONTO                         '.'
#define S_ASPAS_SIMPLES                 '\''
#define S_ASPAS_DUPLAS                  '\"'
#define S_MAIN                          "main"
#define S_IF                            "if"
#define S_ELSE                          "else"
#define S_WHILE                         "while"
#define S_DO                            "do"
#define S_FOR                           "for"
#define S_INT                           "int"
#define S_FLOAT                         "float"
#define S_CHAR                          "char"
#define SPACE                           ' '
#define NEW_LINE                        12
//#define NEW_LINE2                     '\xff'
#define TAB_                            11
#define END_OF_FILE                     300

#endif
