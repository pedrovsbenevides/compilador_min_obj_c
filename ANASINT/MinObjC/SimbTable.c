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

    simb.enderecoRel = countSimbs;
    SIMB[countSimbs] = simb;

    return countSimbs;
}

void setIsArray(int simbIdx)
{
    SIMB[simbIdx].array = TRUE;
}

// comparação de pertencimento a função
// simbIdx => indice do simbolo de mesmo lexema encontrado
// funcIdx => indice da funcao que esta recebendo declaracoes de identificadores
int compFunc(int simbIdx, int funcIdx)
{
    for (int i = simbIdx; i >= 0; i--)
    {
        if (strcmp(SIMB[funcIdx].lexema, SIMB[i].lexema) == 0)
        {
            return 1;
        }
    }

    return 0;
}

// busca identificador e caso encontre verifica pertencimento a alguma função habilitando redeclaração em funções distintas
int compVarInFunc(char lexema[])
{
    int funcIdx = -1;
    for (int i = countSimbs; i >= 0; i--)
    {

        if (SIMB[i].papel == FUNC)
        {
            funcIdx = i;
        }

        if (strcmp(lexema, SIMB[i].lexema) == 0)
        {
            for (int j = i; j >= 0; j--)
            {
                if (SIMB[j].papel == FUNC && strcmp(SIMB[funcIdx].lexema, SIMB[j].lexema) == 0)
                {
                    return TRUE;
                }
            }
        }
    }

    if (funcIdx == -1)
    {
        return TRUE;
    }

    return FALSE;
}

// retorna o tipo da funcao no indice
int getFuncType(int funcIdx)
{
    return SIMB[funcIdx].tipo;
}

int checkIsGlobal(int simbIdx)
{
    return SIMB[simbIdx].escopo == GLOBAL;
}

void setPapel(int simbIdx, int papel)
{
    SIMB[simbIdx].papel = papel;
}

void setEndRelativo(int simbIdx, int label)
{
    SIMB[simbIdx].enderecoRel = label;
}

int findMain()
{
    for (int i = countSimbs; i >= 0; i--)
    {
        if (strcmp("main", SIMB[i].lexema) == 0)
        {
            return i;
        }
    }

    error("funcao Main nao encontrada");
}

int countGlobals()
{
    int globals = 0;
    for (int i = countSimbs; i >= 0; i--)
    {
        if (SIMB[i].escopo == GLOBAL && SIMB[i].papel == VAR)
        {
            globals++;
        }
    }

    return globals;
}

int countArgs(int funcIdx)
{
    int args = 0;
    for (int i = (funcIdx + 1); i <= countSimbs; i++)
    {
        if (SIMB[i].escopo == LOCAL && SIMB[i].papel == PARAM)
        {
            args++;
        }
        else if (SIMB[i].papel == FUNC)
        {
            return args;
        }
    }

    return args;
}

int countLocals(int funcIdx)
{
    int locals = 0;
    for (int i = (funcIdx + 1); i <= countSimbs; i++)
    {
        if (SIMB[i].escopo == LOCAL && SIMB[i].papel == VAR)
        {
            locals++;
        }
        else if (SIMB[i].papel == FUNC)
        {
            return locals;
        }
    }

    return locals;
}

int getEndRelativo(int simbIdx)
{
    return SIMB[simbIdx].enderecoRel;
}

int getEscopo(int simbIdx)
{
    if (SIMB[simbIdx].escopo == GLOBAL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int checkIsFunc(int simbIdx)
{
    return SIMB[simbIdx].papel == FUNC;
}
