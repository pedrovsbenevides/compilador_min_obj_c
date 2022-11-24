#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"
#include "TypeTable.h"
#include "SimbTable.h"

SIMBOLO findSimb(char lexema[])
{
    for (int i = 0; i <= sizeof(SIMB); i++)
    {
        if (strcmp(lexema, SIMB[i].lexema) == 0)
        {
            return SIMB[i];
        }
    }
}

// verificação de escopo, global ou local
int compEsc(int escopo)
{
    for (int i = 0; i <= sizeof(SIMB); i++)
    {
        if (SIMB[i].escopo == escopo)
        {
            return 1;
        }
    }
    return 0;
}

// verificação de contexto
int compCntx(int contexto)
{
    for (int i = 0; i <= sizeof(SIMB); i++)
    {
        if (SIMB[i].papel == contexto)
        {
            return 1;
        }
    }
    return 0;
}

int compTipo(int tipo)
{
    for (int i = 0; i <= sizeof(SIMB); i++)
    {
        if (SIMB[i].tipo == tipo)
        {
            return 1;
        }
    }
    return 0;
}

// inserir
void insertSimb(SIMBOLO simb)
{
    static int countSimbs = 0;

    SIMB[countSimbs] = simb;

    countSimbs++;
}