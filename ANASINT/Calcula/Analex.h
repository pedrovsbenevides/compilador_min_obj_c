#ifndef _ANALEX_     
#define _ANALEX_

#include <stdio.h>

#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT {ID=1, SN, CT_I, FIM_ARQ};
/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira */

enum SINAIS {ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR}; // Sinais válidos da linguagem

enum BOOL {FALSE, TRUE}; 

typedef
    struct { 
        enum TOKEN_CAT cat;    // deve receber uma das constantes de enum TOKEN_CAT
        enum BOOL processado;
        union {     // parte variável do registro
            int codigo;  // para tokens das categorias SN
            char lexema[TAM_MAX_LEXEMA];  // cadeia de caractares que corresponde o nome do token da cat. ID
            int valInt; // valor da constante ineira em tokens da cat. CT_I
        };
    } TOKEN;    // Tipo TOKEN


/* Variaveis globais */
extern TOKEN t;
extern FILE *fd;

/* Assinaturas de funcoes */
TOKEN Analex(FILE *);

#endif
