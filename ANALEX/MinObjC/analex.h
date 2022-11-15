#ifndef ANALEX

#define ANALEX

#define TAM_MAX_LEXEMA 31

enum TOKEN_CAT
{
    ID = 1,
    CT_C,
    CT_F,
    CT_I,
    LT,
    PR,
    SN,
    OP_ARIT,
    OP_LOGIC,
};
/* Onde: ID: Identificador, PR: Palavra Reservada; SN: Sinal; CT_*: Constante numérica ou caracter, LT: cadeia de caracter */

enum PAL_RESERV
{
    CLASS = 1,
    CODE,
    DATA,
    VOID,
    INTERN,
    CHAR,
    INT,
    FLOAT,
    BOOL,
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN,
    DELETE,
    NEW,
};

enum SINAIS
{
    ABRE_PAR = 1,
    FECHA_PAR,
    ABRE_COL,
    FECHA_COL,
    ABRE_CHAVE,
    FECHA_CHAVE,
    PONTO_VIRG,
    DOIS_PONTOS,
    QUATR_PONTOS,
    VIRG,
    CIRCUNFLEXO,
    TIL,
    PONTO,
    E_COMERC
};

// operadores aritmeticos
enum OP_ARIT
{
    ATRIB = 1,
    ADICAO,
    SUBTRACAO,
    MULTIPLIC,
    DIVISAO
};

// operadores logicos
enum OP_LOGIC
{
    IGUAL_A = 1,
    DIFERENTE,
    MENOR_IGUAL,
    MENOR_QUE,
    MAIOR_IGUAL,
    MAIOR_QUE,
    E_LOGIC,
    OU_LOGIC,
    NEGACAO
};

enum CONTROL
{
    FIM_ARQ = 99,
};

typedef

    struct
{

    enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT

    // parte variável do registro
    union
    {

        int codigo; // para tokens das categorias PR e SN

        int indice; // indica o índice da tabela onde o Analex armazenou um literal (cadeia de caracter) para tokens da cat. LT

        char lexema[TAM_MAX_LEXEMA]; // cadeia de caractares que corresponde o nome do token da cat. ID

        int valInt; // valor da constante ineira em tokens da cat. CT_I

        float valFloat; // valor da constante ineira em tokens da cat. CT_F

        char caracter; // caracter do token da cat. CT_C
    };

} TOKEN; // Tipo TOKEN

#endif