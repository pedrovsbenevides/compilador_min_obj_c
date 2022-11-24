#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"
#include "TypeTable.h"

int findType(char lexema[])
{
    for (int i = 0; i <= sizeof(TYPES) / sizeof(TYPES[0]); i++)
    {
        if (strcmp(lexema, TYPES[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// inserir
void insertType(char lexema[])
{
    static int countTypes = 0;

    strcpy(TYPES[countTypes], lexema);

    countTypes++;
}