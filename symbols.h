#ifndef SYMBOLS_H_
#define SYMBOLS_H_

typedef enum{
	false, true
}boolean;

/* reserved words */
#define MAIN 0
#define IF 1
#define ELSE 2
#define WHILE 3
#define DO 4
#define FOR 5
#define INT 6
#define FLOAT 7
#define CHAR 8

/* oprelacional */
#define MENOR 9
#define MAIOR 10
#define MENOR_IGUAL 11
#define MAIOR_IGUAL 12
#define IGUAL_COMPARACAO 13
#define DIFERENTE_COMPARACAO 14

/* oparitimetico */
#define SOMA 15
#define SUBTRACAO 16
#define MULTIPLICACAO 17
#define DIVISAO 18
#define IGUAL_ATRIBUICAO 19

/* special characters */
#define ABRE_PARENTESES 20
#define FECHA_PARENTESES 21
#define ABRE_CHAVES 22
#define FECHA_CHAVES 23
#define VIRGULA 24
#define PONTO_VIRGULA 25

/* letra */
#define LETRA 26

/* digito */
#define DIGITO 27

/* identificador */
#define ID 28

/* errors*/
#define ERROR_PARAMETERS_NOT_PASSED 29
#define ERROR_FILE_NOT_RECOGNIZED 30

#endif