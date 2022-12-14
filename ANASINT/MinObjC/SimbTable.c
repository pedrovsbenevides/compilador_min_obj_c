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
    return FALSE;
}

// verificação de escopo, global ou local
int compEsc(int escopo, int simbIdx)
{
    if (SIMB[simbIdx].escopo == escopo)
    {
        return TRUE;
    }

    return FALSE;
}

// verificação de contexto
int compCntx(int contexto, int simbIdx)
{
    if (SIMB[simbIdx].papel == contexto)
    {
        return TRUE;
    }

    return FALSE;
}

int compTipo(int tipo, int simbIdx)
{

    if (SIMB[simbIdx].tipo == tipo)
    {
        return TRUE;
    }

    return FALSE;
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
