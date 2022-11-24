#ifndef _SIMB_TABLE_
#define _SIMB_TABLE_

enum ESCOPO
{
    GLOBAL = 1,
    LOCAL
};

enum PAPEL
{
    VAR = 1,
    FUNCTION,
    OBJ,
    PARAM
};

typedef

    struct
{
    union
    {
        char lexema[TAM_MAX_LEXEMA]; // cadeia de caractares que corresponde o nome
                                     // do token da cat. ID

        enum PAL_RESERV tipo; // para identificadores com tipos padrao

        char tipo_def[TAM_MAX_LEXEMA]; // para identificadores com tipos definidos
                                       // pelo usuario, o tipo é o proprio lexema

        enum ESCOPO escopo; // escopo do identificador

        enum PAPEL papel; // papel desempenhado pelo identificador, contexto em que
                          // esta inserido

        int indice; // indica o índice on de foi guardado na tabela de simbolos

        int valInt; // valor caso tipo INT/BOOL

        float valFloat; // valor caso tipo FLOAT

        char caracter; // valor caso tipo CHAR

        int ponteiro; // indicador de se identificador é ponteiro ou não;

        int enderecoRel; // endereço relativo

        int idx; // indice na tabela de simbolos

        int deslocamento; // deslocamento na maq de pilha
    };

} SIMBOLO; // Tipo SIMBOLO

extern SIMBOLO SIMB[100]; // tabela de simbolos

SIMBOLO findSimb(char lexema[]);
int compEsc(int escopo);
int compCntx(int contexto);
int compTipo(int tipo);
void insertSimb(SIMBOLO simb);

#endif