#ifndef _ANASINT_
#define _ANASINT_

#include <stdio.h>
#include "AnaLex.h"

/* Variaveis globais */
extern TOKEN tk;
extern FILE *fd, *fd_out;
extern int countLabels;

extern enum BOOL mostraArvore;
extern char TABS[200];

/* Assinaturas das funcoes */
void Prog();
void ObjDef();
void DataSec();
void DeclVar();
void SignFunc();
void DeclVarFunc();
void DeclListVar(int listSimbType);
void DeclListFunc();
void DeclArrayVar(int varIdx);
void MethSec();
void FuncProt();
void Func();
void Cmd();
void RestAtrib();
void TiposParam();
void Atrib();
char Tipo();
void Expr();
void ExprSimp();
void OpRel();
void Fator();
void Termo();

#endif // ANASINT