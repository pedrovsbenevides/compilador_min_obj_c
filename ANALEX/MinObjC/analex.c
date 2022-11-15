//Autor: Pedro Victor Santana Benevides

#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <string.h>

#include "Analex.h"

#define TAM_LEXEMA 50

#define TAM_NUM 20

#define MAX_LIT 50 // quantidade maxima de literais

char tableLit[MAX_LIT][TAM_LEXEMA] = {""};
int lti = 0; // controle do indice de literais

void error(char msg[])
{

    printf("%s\n", msg);

    exit(1);
}

TOKEN AnaLex(FILE *fd)
{

    int estado;

    char lexema[TAM_LEXEMA] = "";

    int tamL = 0;

    char digitos[TAM_NUM] = "";

    int tamD = 0;

    TOKEN t;

    estado = 0;

    while (1)
    {

        char c = fgetc(fd);

        switch (estado)
        {

        case 0:
            if (c == ' ' || c == '\t' || c == '\n')
            {
                estado = 0;
            }

            else if (c == '/')
            {
                // sinal de divisao ou inicio de comentario - inicializa

                estado = 1;
            }

            else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                // inicio de identificador - inicializa lexema

                estado = 4;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }

            else if (c >= '1' && c <= '9')
            {
                // inicio de constante inteira - inicializa digitos

                estado = 6;

                digitos[tamD] = c;

                digitos[++tamD] = '\0';
            }

            else if (c == '\'')
            {
                // apóstrofro inicio de declaracao de char
                estado = 10;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }

            else if (c == '"')
            {
                // aspas inicio de declaracao de string
                estado = 14;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }

            else if (c == '+')
            {
                // sinal de adicao - monta e devolve token

                estado = 20;

                t.cat = OP_ARIT;

                t.codigo = ADICAO;

                return t;
            }

            else if (c == '-')
            {
                // sinal de subtracao - monta e devolve token

                estado = 19;

                t.cat = OP_ARIT;

                t.codigo = SUBTRACAO;

                return t;
            }

            else if (c == '*')
            {
                // sinal de multiplicacao - monta e devolve token

                estado = 18;

                t.cat = OP_ARIT;

                t.codigo = MULTIPLIC;

                return t;
            }

            else if (c == '=')
            {
                // sinal de atribuicao ou inicio de operador de comparação 'igual a'

                estado = 9;
            }

            else if (c == '!')
            {
                // sinal de negacao ou inicio de operador de diferença

                estado = 27;

                t.cat = OP_LOGIC;
            }

            else if (c == '>')
            {
                // sinal de 'maior que' ou inicio de operador de maior ou igual

                estado = 28;

                t.cat = OP_LOGIC;
            }

            else if (c == '<')
            {
                // sinal de 'menor que' ou inicio de operador de menor ou igual

                estado = 29;

                t.cat = OP_LOGIC;
            }

            else if (c == '&')
            {
                // inicio de operador logico E ou indicador de ponteiro E Comercial

                estado = 41;
            }

            else if (c == '|')
            {
                // inicio de operador logico OU

                estado = 43;

                t.cat = OP_LOGIC;
            }

            else if (c == '(')
            {
                // sinal de abre parenteses - monta e devolve token

                estado = 21;

                t.cat = SN;

                t.codigo = ABRE_PAR;

                return t;
            }

            else if (c == ')')
            {
                // sinal de fecha parenteses - monta e devolve token

                estado = 22;

                t.cat = SN;

                t.codigo = FECHA_PAR;

                return t;
            }

            else if (c == '[')
            {
                // sinal de abre colchetes - monta e devolve token

                estado = 23;

                t.cat = SN;

                t.codigo = ABRE_COL;

                return t;
            }

            else if (c == ']')
            {
                // sinal de fecha colchetes - monta e devolve token

                estado = 24;

                t.cat = SN;

                t.codigo = FECHA_COL;

                return t;
            }

            else if (c == '{')
            {
                // sinal de abre chave - monta e devolve token

                estado = 25;

                t.cat = SN;

                t.codigo = ABRE_CHAVE;

                return t;
            }

            else if (c == '}')
            {
                // sinal de fecha chave - monta e devolve token

                estado = 26;

                t.cat = SN;

                t.codigo = FECHA_CHAVE;

                return t;
            }

            else if (c == '^')
            {
                // sinal de circunflexo - monta e devolve token

                estado = 36;

                t.cat = SN;

                t.codigo = CIRCUNFLEXO;

                return t;
            }

            else if (c == ',')
            {
                // sinal de virgula - monta e devolve token

                estado = 35;

                t.cat = SN;

                t.codigo = VIRG;

                return t;
            }

            else if (c == ';')
            {
                // sinal de ponto e virgula - monta e devolve token

                estado = 37;

                t.cat = SN;

                t.codigo = PONTO_VIRG;

                return t;
            }

            else if (c == ':')
            {
                // sinal de dois pontos ou inicio de quatro pontos - inicializa

                estado = 38;

                t.cat = SN;
            }

            else if (c == '~')
            {
                // sinal de til - monta e devolve token

                estado = 45;

                t.cat = SN;

                t.codigo = TIL;

                return t;
            }

            else if (c == '.')
            {
                // sinal de ponto - monta e devolve token

                estado = 47;

                t.cat = SN;

                t.codigo = PONTO;

                return t;
            }

            else if (c == EOF)
            {
                // fim da expressao encontrado
                t.cat = FIM_ARQ;

                return t;
            }

            else
            {
                error("Caracter invalido na expressao!");
                // sem transicao valida no AFD
            }

            break;

        case 1:
            if (c == '*')
            {
                // inicio de comentario
                estado = 2;
            }

            else
            {
                // transicao OUTRO* do estado 1 do AFD

                // monta token divisao e retorna
                estado = 17;

                ungetc(c, fd);

                t.cat = OP_ARIT;

                t.codigo = DIVISAO;

                return t;
            }

            break;
        case 2:
            if (c == '*')
            {
                estado = 3;
            }
            else
            {
                estado = 2;
            }
            break;

        case 3:
            if (c == '/')
            {
                estado = 0;
            }
            else if (c == '*')
            {
                estado = 3;
            }
            else
            {
                estado = 2;
            }
            break;
        case 4:
            if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_'))
            {

                estado = 4;

                lexema[tamL] = c; // acumula caracteres lidos em lexema

                lexema[++tamL] = '\0';
            }

            else
            {
                // transicao OUTRO* do estado 4 do AFD

                estado = 5; // monta token

                ungetc(c, fd);
                strcpy(t.lexema, lexema);

                // verificacao de palavras reservadas
                if (strcmp("class", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = CLASS;

                    return t;
                }

                else if (strcmp("code", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = CODE;

                    return t;
                }

                else if (strcmp("data", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = DATA;

                    return t;
                }

                else if (strcmp("void", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = VOID;

                    return t;
                }

                else if (strcmp("intern", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = INTERN;

                    return t;
                }

                else if (strcmp("char", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = CHAR;

                    return t;
                }

                else if (strcmp("int", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = INT;

                    return t;
                }

                else if (strcmp("float", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = FLOAT;

                    return t;
                }

                else if (strcmp("bool", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = BOOL;

                    return t;
                }

                else if (strcmp("if", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = IF;

                    return t;
                }

                else if (strcmp("else", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = ELSE;

                    return t;
                }

                else if (strcmp("while", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = WHILE;

                    return t;
                }

                else if (strcmp("for", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = FOR;

                    return t;
                }

                else if (strcmp("return", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = RETURN;

                    return t;
                }

                else if (strcmp("delete", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = DELETE;

                    return t;
                }

                else if (strcmp("new", lexema) == 0)
                {
                    t.cat = PR;
                    t.codigo = NEW;

                    return t;
                }
                else
                {
                    // montagem do identificador
                    t.cat = ID;

                    return t;
                }
            }
            break;
        case 6:
            if (c >= '0' && c <= '9')
            {
                estado = 6;

                digitos[tamD] = c; // acumula digitos lidos na variavel digitos

                digitos[++tamD] = '\0';
            }
            else if (c == '.')
            {

                estado = 8;

                digitos[tamD] = c;

                digitos[++tamD] = '\0';
            }

            else
            {
                // transicao OUTRO* do estado 6 do AFD

                estado = 7; // monta token constante inteira e retorna

                ungetc(c, fd);

                t.cat = CT_I;

                t.valInt = atoi(digitos);

                return t;
            }
            break;
        case 8:
            if (c >= '0' && c <= '9')
            {

                estado = 8;

                digitos[tamD] = c; // acumula digitos das casas decimais

                digitos[++tamD] = '\0';
            }

            else
            {
                // transicao OUTRO* do estado 8 do AFD

                estado = 7; // monta token constante float e retorna

                ungetc(c, fd);

                t.cat = CT_F;

                t.valFloat = atof(digitos);

                return t;
            }
            break;
        case 9:
            if (c == '=')
            {
                estado = 33;

                t.cat = OP_LOGIC;
                t.codigo = IGUAL_A;

                return t;
            }
            else
            {
                estado = 34;

                ungetc(c, fd);

                t.cat = OP_ARIT;
                t.codigo = ATRIB;

                return t;
            }

            break;
        case 10:
            t.caracter = c;

            if (c == '\\')
            {
                estado = 12;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }

            else if (c == '\'')
            {

                error("Caracter invalido na expressao!");
                // sem transicao valida no AFD
            }

            else
            {
                estado = 11;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }
            break;
        case 11:
            if (c == '\'')
            {
                estado = 13;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';

                t.cat = CT_C;

                strcpy(t.lexema, lexema);

                return t;
            }
            else
            {
                error("Caracter invalido na expressao!");
                // sem transicao valida no AFD
            }
            break;
        case 12:
            if (c == 'n' || c == '0')
            {
                estado = 11;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }
            else if (c == '\'')
            {
                estado = 13;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';

                t.cat = CT_C;

                strcpy(t.lexema, lexema);

                return t;
            }
            else
            {
                error("Caracter invalido na expressao!");
                // sem transicao valida no AFD
            }
            break;
        case 14:
            if (c == '"')
            {
                estado = 16;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';

                t.cat = LT;

                strcpy(t.lexema, lexema);

                return t;
            }
            else if (c == '\n')
            {
                error("Caracter invalido na expressao!");
                // sem transicao valida no AFD
            }
            else
            {
                estado = 15;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }
            break;
        case 15:
            if (c == '\n')
            {
                error("Caracter invalido na expressao!");
                // sem transicao valida no AFD
            }
            else if (c == '"')
            {
                estado = 16;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';

                strcpy(tableLit[lti], lexema);

                t.cat = LT;

                t.indice = lti;

                lti++;

                strcpy(t.lexema, lexema);

                return t;
            }
            else
            {
                estado = 15;

                lexema[tamL] = c;

                lexema[++tamL] = '\0';
            }

            break;
        case 27:
            if (c == '=')
            {
                estado = 33;

                t.codigo = DIFERENTE;

                return t;
            }
            else
            {
                ungetc(c, fd);

                t.codigo = NEGACAO;

                return t;
            }

            break;
        case 28:
            if (c == '=')
            {
                estado = 33;

                t.codigo = MAIOR_IGUAL;

                return t;
            }
            else
            {
                ungetc(c, fd);

                t.codigo = MAIOR_QUE;

                return t;
            }

            break;
        case 29:
            if (c == '=')
            {
                estado = 33;

                t.codigo = MENOR_IGUAL;

                return t;
            }
            else
            {
                ungetc(c, fd);

                t.codigo = MENOR_QUE;

                return t;
            }

            break;
        case 38:
            if (c == ':')
            {
                estado = 40;

                t.codigo = QUATR_PONTOS;

                return t;
            }
            else
            {
                estado = 39;

                ungetc(c, fd);

                t.codigo = DOIS_PONTOS;

                return t;
            }

            break;
        case 41:
            if (c == '&')
            {
                // monta E LOGICO
                estado = 42;

                t.cat = OP_LOGIC;

                t.codigo = E_LOGIC;

                return t;
            }
            else
            {
                // monta indicador de ponteiro - E COMERCIAL
                estado = 46;

                t.cat = SN;

                t.codigo = E_COMERC;

                return t;
            }
            break;
        case 43:
            if (c == '|')
            {
                estado = 44;

                t.codigo = OU_LOGIC;

                return t;
            }
            else
            {
                error("Caracter invalido na expressao!");
            }
            break;
        }
    }
}

int main()
{

    FILE *fd;

    TOKEN tk;

    if ((fd = fopen("testAnalex.dat", "r")) == NULL)

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

    return 0;
}