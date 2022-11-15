#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Analex.h"
#include "Anasint.h"
#include "Funcaux.h"

/* Variaveis globais */
TOKEN t;
FILE *fd;
enum BOOL mostraArvore = TRUE;
char TABS[200] = "";


void TestaAnalex() {

    if ((fd=fopen("expressao.dat", "r")) == NULL)
        erro("Arquivo de entrada da expressao nao encontrado!");

    while (1) {
        t = Analex(fd);
        switch (t.cat) {
            case ID: printf("<ID, %s> ", t.lexema);
                     break;
            case SN: switch (t.codigo) {
                        case ADICAO: printf("<SN, ADICAO> ");
                        break;
                        case SUBTRACAO: printf("<SN, SUBTRACAO> ");
                        break;
                        case MULTIPLIC: printf("<SN, MULTIPLICACAO> ");
                        break;
                        case DIVISAO: printf("<SN, DIVISAO> ");
                        break;
                        case ATRIB: printf("<SN, ATRIBUICAO> ");
                        break;
                        case ABRE_PAR: printf("<SN, ABRE_PARENTESES> ");
                        break;
                        case FECHA_PAR: printf("<SN, FECHA_PARENTESES> ");
                        break;
                     }
                     break;
            case CT_I: printf("<CT_I, %d> ", t.valInt);
                       break;
            case FIM_ARQ: printf("\nFim da expressao encontrado.\n");
        }
        if (t.cat == FIM_ARQ) break;
    }
    fclose(fd);
}

void TestaAnasint() {

    if ((fd=fopen("expressao.dat", "r")) == NULL)
        erro("Arquivo de entrada da expressao nao encontrado!");

    Atrib();

    if (t.cat==FIM_ARQ)
        printf("Resultado> Expressao OK!\n");
    else {
        switch (t.cat) {
            case ID: printf("<ID, %s> ", t.lexema);
                     break;
            case SN: switch (t.codigo) {
                        case ADICAO: printf("<SN, ADICAO> ");
                        break;
                        case SUBTRACAO: printf("<SN, SUBTRACAO> ");
                        break;
                        case MULTIPLIC: printf("<SN, MULTIPLICACAO> ");
                        break;
                        case DIVISAO: printf("<SN, DIVISAO> ");
                        break;
                        case ATRIB: printf("<SN, ATRIBUICAO> ");
                        break;
                        case ABRE_PAR: printf("<SN, ABRE_PARENTESES> ");
                        break;
                        case FECHA_PAR: printf("<SN, FECHA_PARENTESES> ");
                        break;
                     }
                     break;
            case CT_I: printf("<CT_I, %d> ", t.valInt);
                       break;
            case FIM_ARQ: ;
        }
        erro("Erro de sintaxe!");
    }
    fclose(fd);
}

int main() {

    printf("\n[Analise Lexica -------------------]\n");
    TestaAnalex();
    printf("\n[Analise Sintatica ----------------]\n");
    TestaAnasint();

}
