#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"
#include "TypeTable.h"
#include "SimbTable.h"

static int countSimbs = 0;

int findSimb(char lexema[])
{
    for (int i = countSimbs; i >= 0; i--)
    {
        if (strcmp(lexema, SIMB[i].lexema) == 0)
        {
            return i;
        }
    }
    return 0;
}

// verificação de escopo, global ou local
int compEsc(int escopo, int simbIdx)
{
    if (SIMB[simbIdx].escopo == escopo)
    {
        return 1;
    }

    return 0;
}

// verificação de contexto
int compCntx(int contexto)
{
    for (int i = countSimbs; i >= 0; i--)
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
    for (int i = countSimbs; i >= 0; i--)
    {
        if (SIMB[i].tipo == tipo)
        {
            return 1;
        }
    }
    return 0;
}

// inserir
int insertSimb(SIMBOLO simb)
{
    countSimbs++;

    SIMB[countSimbs] = simb;

    return countSimbs;
}

void setIsArray(int simbIdx)
{
    SIMB[simbIdx].array = TRUE;
}
