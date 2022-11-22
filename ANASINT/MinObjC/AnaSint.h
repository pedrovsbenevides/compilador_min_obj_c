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
void Prog();
void ObjDef();
void DataSec();
void DeclVar();
void SignFunc();
void DeclVarFunc();
void DeclListVar();
void DeclListFunc();
void DeclArrayVar();
void MethSec();
void FuncProt();
void Func();
void Cmd();
void TiposParam();
void Atrib();
void Tipo();
void Expr();
void Termo();
void Resto();
void Sobra();
void Fator();

#endif // ANASINT