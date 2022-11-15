#ifndef _FUNC_AUX_
#define _FUNC_AUX_

enum MOVIM
{
    AVANCA,
    MANTEM,
    RETROCEDE
};

extern enum BOOL mostraArvore;
extern char TABS[200];

void error(char msg[]);
void PrintNodo(char info[], int movim);
void PrintNodoInt(int val, int movim);

#endif