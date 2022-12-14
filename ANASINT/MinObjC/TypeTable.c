#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"
#include "TypeTable.h"

static int countTypes = 0;

int findType(char lexema[])
{
    for (int i = countTypes; i >= 0; i--)
    {
        if (strcmp(lexema, TYPES[i].lexema) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

// inserir
int insertType(SIMBOLO type)
{
    countTypes++;

    TYPES[countTypes] = type;

    return countTypes;
}