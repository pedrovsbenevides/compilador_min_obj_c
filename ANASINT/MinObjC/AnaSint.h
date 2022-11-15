#ifndef _ANASINT_
#define _ANASINT_

#include <stdio.h>
#include "AnaLex.h"

/* Variaveis globais */
extern TOKEN tk;
extern FILE *fd;

extern enum BOOL mostraArvore;
extern char TABS[200];

/* Assinaturas das funcoes */
void Atrib();
void Tipo();
void Expr();
void Termo();
void Resto();
void Sobra();
void Fator();

#endif // ANASINT