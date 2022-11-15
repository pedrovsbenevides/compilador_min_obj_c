#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"

/* Variaveis globais */
TOKEN tk;
FILE *fd;
enum BOOL mostraArvore = TRUE;
char TABS[200] = "";

void TestaAnalex()
{
    if ((fd = fopen("comand.dat", "r")) == NULL)

        error("Arquivo de entrada de comandos MinObjC nao encontrado!");

    while (1)
    {

        tk = AnaLex(fd);

        switch (tk.cat)
        {

        case ID:
            printf("<ID, %s>\n", tk.lexema);

            break;

        case CT_C:
            printf("<CT_C, %s>\n", tk.lexema);

            break;

        case CT_F:
            printf("<CT_F, %f>\n", tk.valFloat);

            break;

        case CT_I:
            printf("<CT_I, %d>\n", tk.valInt);

            break;

        case LT:
            printf("<LT, %s>\n", tk.lexema);

            break;

        case PR:
            switch (tk.codigo)
            {
            case CLASS:
                printf("<PR, CLASS>\n");

                break;
            case CODE:
                printf("<PR, CODE>\n");

                break;
            case DATA:
                printf("<PR, DATA>\n");

                break;
            case VOID:
                printf("<PR, VOID>\n");

                break;
            case INTERN:
                printf("<PR, INTERN>\n");

                break;
            case CHAR:
                printf("<PR, CHAR>\n");

                break;
            case INT:
                printf("<PR, INT>\n");

                break;
            case FLOAT:
                printf("<PR, FLOAT>\n");

                break;
            case BOOL:
                printf("<PR, BOOL>\n");

                break;
            case IF:
                printf("<PR, IF>\n");

                break;
            case ELSE:
                printf("<PR, ELSE>\n");

                break;
            case WHILE:
                printf("<PR, WHILE>\n");

                break;
            case FOR:
                printf("<PR, FOR>\n");

                break;
            case RETURN:
                printf("<PR, RETURN>\n");

                break;
            case DELETE:
                printf("<PR, DELETE>\n");

                break;
            case NEW:
                printf("<PR, NEW>\n");

                break;
            }
            break;
        case SN:
            switch (tk.codigo)
            {
            case ABRE_PAR:
                printf("<SN, ABRE_PARENTESES>\n");

                break;

            case FECHA_PAR:
                printf("<SN, FECHA_PARENTESES>\n");

                break;

            case PONTO_VIRG:
                printf("<SN, PONTO_VIRG>\n");
                break;

            case DOIS_PONTOS:
                printf("<SN, DOIS_PONTOS>\n");
                break;

            case QUATR_PONTOS:
                printf("<SN, QUATR_PONTOS>\n");
                break;

            case VIRG:
                printf("<SN, VIRG>\n");
                break;

            case ABRE_COL:
                printf("<SN, ABRE_COL>\n");
                break;

            case FECHA_COL:
                printf("<SN, FECHA_COL>\n");
                break;

            case ABRE_CHAVE:
                printf("<SN, ABRE_CHAVE>\n");
                break;

            case FECHA_CHAVE:
                printf("<SN, FECHA_CHAVE>\n");
                break;

            case CIRCUNFLEXO:
                printf("<SN, CIRCUNFLEXO>\n");
                break;

            case TIL:
                printf("<SN, TIL>\n");
                break;

            case PONTO:
                printf("<SN, PONTO>\n");
                break;

            case E_COMERC:
                printf("<SN, E_COMERC>\n");
                break;
            }

            break;
        case OP_ARIT:
            switch (tk.codigo)
            {
            case ATRIB:
                printf("<SN, ATRIBUICAO>\n");
                break;

            case ADICAO:
                printf("<SN, ADICAO>\n");
                break;

            case SUBTRACAO:
                printf("<SN, SUBTRACAO>\n");
                break;

            case MULTIPLIC:
                printf("<SN, MULTIPLICACAO>\n");
                break;

            case DIVISAO:
                printf("<SN, DIVISAO>\n");
                break;
            }

            break;
        case OP_LOGIC:
            switch (tk.codigo)
            {

            case IGUAL_A:
                printf("<SN, IGUAL_A>\n");
                break;

            case DIFERENTE:
                printf("<SN, DIFERENTE>\n");
                break;

            case NEGACAO:
                printf("<SN, NEGACAO>\n");
                break;

            case MENOR_IGUAL:
                printf("<SN, MENOR_IGUAL>\n");
                break;

            case MENOR_QUE:
                printf("<SN, MENOR_QUE>\n");
                break;

            case MAIOR_QUE:
                printf("<SN, MAIOR_QUE>\n");
                break;

            case MAIOR_IGUAL:
                printf("<SN, MAIOR_IGUAL>\n");
                break;

            case E_LOGIC:
                printf("<SN, E_LOGIC>\n");
                break;

            case OU_LOGIC:
                printf("<SN, OU_LOGIC>\n");
                break;
            }

            break;
        case FIM_ARQ:
            printf("\nFim da expressao encontrado.\n");
        }

        if (tk.cat == FIM_ARQ)
            break;
    }

    fclose(fd);
}

void TestaAnasint()
{

    if ((fd = fopen("comand.dat", "r")) == NULL)
        error("Arquivo de entrada de comandos nao encontrado!");

    Atrib();

    if (tk.cat == FIM_ARQ)
        printf("Resultado> Expressao OK!\n");
    else
    {
        switch (tk.cat)
        {
        case ID:
            printf("<ID, %s> ", tk.lexema);
            break;
        case OP_ARIT:
            switch (tk.codigo)
            {
            case ADICAO:
                printf("<OP_ARIT, ADICAO> ");
                break;
            case SUBTRACAO:
                printf("<OP_ARIT, SUBTRACAO> ");
                break;
            case MULTIPLIC:
                printf("<OP_ARIT, MULTIPLICACAO> ");
                break;
            case DIVISAO:
                printf("<OP_ARIT, DIVISAO> ");
                break;
            case ATRIB:
                printf("<OP_ARIT, ATRIBUICAO> ");
                break;
            }
        case SN:
            switch (tk.codigo)
            {
            case ABRE_PAR:
                printf("<SN, ABRE_PARENTESES> ");
                break;
            case FECHA_PAR:
                printf("<SN, FECHA_PARENTESES> ");
                break;
            }
            break;
        case CT_I:
            printf("<CT_I, %d> ", tk.valInt);
            break;
        case PR:
            switch (tk.codigo)
            {
            case NEW:
                printf("<PR, NEW> ");
                break;
            case INT:
                printf("<PR, INT> ");
                break;

            case CHAR:
                printf("<PR, CHAR> ");
                break;

            case FLOAT:
                printf("<PR, FLOAT> ");
                break;

            case BOOL:
                printf("<PR, BOOL> ");
                break;
            }
        case FIM_ARQ:;
        }
        error("Erro de sintaxe!");
    }
    fclose(fd);
}

int main()
{

    printf("\n[Analise Lexica -------------------]\n");
    TestaAnalex();
    printf("\n[Analise Sintatica ----------------]\n");
    TestaAnasint();
    return 0;
}
