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
    FUNC,
    OBJ,
    PARAM
};

typedef

    struct
{
    char lexema[TAM_MAX_LEXEMA]; // cadeia de caractares que corresponde o nome
                                 // do token da cat. ID
    enum ESCOPO escopo;          // escopo do identificador

    enum PAPEL papel; // papel desempenhado pelo identificador, contexto em que
                      // esta inserido

    int enderecoRel; // endereço relativo

    int deslocamento; // deslocamento na maq de pilha

    int ponteiro; // indicador de se identificador é ponteiro ou não;

    int array; // indicador de se identificador é array ou não;

    int referencia; // indicador de se identificador é passado por referencia ou não;
    union
    {
        int valInt; // valor caso tipo INT/BOOL

        float valFloat; // valor caso tipo FLOAT

        char caracter; // valor caso tipo CHAR
    };

    union
    {
        enum PAL_RESERV tipo; // para identificadores com tipos padrao

        char tipo_def[TAM_MAX_LEXEMA]; // para identificadores com tipos definidos
                                       // pelo usuario, o tipo é o proprio lexema
    };

} SIMBOLO; // Tipo SIMBOLO

extern SIMBOLO SIMB[100]; // tabela de simbolos

int findSimb(char lexema[]);
int compEsc(int escopo, int simbIdx);
int compCntx(int contexto, int simbIdx);
int compTipo(int tipo, int simbIdx);
int insertSimb(SIMBOLO simb);
void setIsArray(int simbIdx);
int compFunc(int simbIdx, int funcIdx);
int getFuncType(int funcIdx);
int compVarInFunc(char lexema[]);
int checkIsGlobal(int simbIdx);
void setPapel(int simbIdx, int papel);

#endif