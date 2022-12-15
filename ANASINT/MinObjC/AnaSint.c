#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"
#include "TypeTable.h"
#include "SimbTable.h"

#define GLOBAL 1;
#define LOCAL 2;

char tipoTabSimb;
int escopo_atual = GLOBAL;

void Prog()
{
	char cmdObj[20];

	fputs("INIP\n", fd_out);

	if (mostraArvore)
		PrintNodo("<Prog>", AVANCA);

	tk = AnaLex(fd);
	while (tk.cat != FIM_ARQ)
	{

		if (tk.cat == PR && tk.codigo == CLASS)
		{
			if (mostraArvore)
				PrintNodo("CLASS", MANTEM);
			tk.processado = TRUE;

			ObjDef();
		}
		else
		{
			DeclVarFunc();
		}

		if (tk.processado)
			tk = AnaLex(fd);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);

	int mainIdx = findMain();
	sprintf(cmdObj, "CALL L%d\n", getEndRelativo(mainIdx));
	fputs(cmdObj, fd_out);

	sprintf(cmdObj, "DMEM %d\n", countGlobals());
	fputs(cmdObj, fd_out);

	fputs("HALT\n", fd_out);
}

void ObjDef()
{
	if (mostraArvore)
		PrintNodo("<ObjDef>", AVANCA);

	if (tk.processado)
	{
		tk = AnaLex(fd);
	}

	// identificador de classe
	if (tk.cat == ID)
	{
		SIMBOLO newType;
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		tk.processado = TRUE;

		int checkType = findType(tk.lexema);
		PrintNodoInt(checkType, MANTEM);
		if (checkType)
		{
			error("Redeclaracao de OBJ");
		}
		else
		{
			newType.papel = OBJ;
			newType.escopo = escopo_atual;
			strcpy(newType.lexema, tk.lexema);

			insertType(newType);
		}
	}
	else
	{
		error("identificador de classe esperado");
	}

	// abertura de chaves
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == SN && tk.codigo == ABRE_CHAVE)
	{
		escopo_atual = LOCAL;
		if (mostraArvore)
			PrintNodo("{", MANTEM);
		tk.processado = TRUE;
		DataSec();
		MethSec();

		if (tk.processado)
		{
			tk = AnaLex(fd);
		}
		if (tk.cat == SN && tk.codigo == FECHA_CHAVE)
		{
			if (mostraArvore)
				PrintNodo("}", MANTEM);
			tk.processado = TRUE;
			escopo_atual = GLOBAL;
		}
		else
		{
			error("fechamento de chave esperado");
		}
	}
	else
	{
		error("abre chaves esperado");
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void DataSec()
{
	if (mostraArvore)
		PrintNodo("<DataSec>", AVANCA);

	// PR DATA
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == PR && tk.codigo == DATA)
	{
		if (mostraArvore)
			PrintNodo("DATA", MANTEM);
		tk.processado = TRUE;
	}
	else
	{
		error("palavra reservada 'data' esperado");
	}

	// dois pontos
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == SN && tk.codigo == DOIS_PONTOS)
	{
		if (mostraArvore)
			PrintNodo(":", MANTEM);
		tk.processado = TRUE;
		// declaracao de variaveis da classe
		DeclVar();

		// sequencia de declListVar depois do ponto e virgula
		tk = AnaLex(fd);
		while (tk.cat == PR && (tk.codigo == VOID || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL))
		{
			DeclVar();

			// token apos ponto e virgula
			if (tk.processado)
			{
				tk = AnaLex(fd);
			}
		}
		if (tk.cat == ID)
		{
			// tratar tipo_obj
		}
	}
	else
	{
		error("dois pontos esperado");
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void DeclVar()
{
	SIMBOLO newSimb;
	int newSimbIdx;
	int newSimbType;
	newSimb.ponteiro = FALSE;
	newSimb.array = FALSE;

	if (mostraArvore)
		PrintNodo("<DeclVar>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == PR && tk.codigo == VOID)
	{
		if (mostraArvore)
			PrintNodo("VOID", MANTEM);
		tk.processado = TRUE;
		newSimb.tipo = VOID;
	}
	else
	{
		switch (Tipo())
		{
		case 'i':
			newSimbType = INT;
			break;
		case 'f':
			newSimbType = FLOAT;
			break;
		case 'c':
			newSimbType = CHAR;
			break;
		case 'b':
			newSimbType = BOOL;
			break;
		}
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
	{
		if (mostraArvore)
			PrintNodo("^", MANTEM);
		tk.processado = TRUE;
		newSimb.ponteiro = TRUE;
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == ID)
	{
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		tk.processado = TRUE;

		int checkSimb = findSimb(tk.lexema);
		int checkType = findType(tk.lexema);
		if (checkSimb)
		{
			if (compEsc(escopo_atual, checkSimb))
			{
				error("Redeclaracao de identificador");
			}
		}
		else if (checkType)
		{
			error("Redeclaracao de OBJ");
		}
		else
		{
			newSimb.escopo = escopo_atual;
			newSimb.tipo = newSimbType;
			newSimbIdx = insertSimb(newSimb);
		}
	}
	else
	{
		error("identificador esperado");
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == PONTO_VIRG)
	{
		if (mostraArvore)
			PrintNodo(";", MANTEM);
		tk.processado = TRUE;

		if (newSimb.tipo == VOID && !newSimb.ponteiro)
		{
			error("ERRO: Apenas ponteiros podem ter tipo VOID");
		}
	}
	else if (tk.cat == SN && tk.codigo == VIRG)
	{
		DeclListVar(newSimbType);

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat != SN && tk.codigo != PONTO_VIRG)
		{
			error("ponto e virgula esperado");
		}
		else
		{
			if (mostraArvore)
				PrintNodo(";", MANTEM);
			tk.processado = TRUE;
		}
	}
	else if (tk.cat == SN && tk.codigo == ABRE_COL)
	{
		DeclArrayVar(newSimbIdx);

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == VIRG)
		{
			DeclListVar(newSimbType);
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat != SN && tk.codigo != PONTO_VIRG)
		{
			error("ponto e virgula esperado");
		}
		else
		{
			if (mostraArvore)
				PrintNodo(";", MANTEM);
			tk.processado = TRUE;
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void SignFunc()
{
	SIMBOLO newSimb;
	int newSimbIdx;
	int newSimbType;
	newSimb.ponteiro = FALSE;
	newSimb.array = FALSE;
	newSimb.papel = FUNC;

	if (mostraArvore)
		PrintNodo("<SignFunc>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);

	if (tk.cat == PR && tk.codigo == VOID)
	{
		if (mostraArvore)
			PrintNodo("VOID", MANTEM);
		tk.processado = TRUE;
		newSimbType = VOID;
	}
	else
	{
		switch (Tipo())
		{
		case 'i':
			newSimbType = INT;
			break;
		case 'f':
			newSimbType = FLOAT;
			break;
		case 'c':
			newSimbType = CHAR;
			break;
		case 'b':
			newSimbType = BOOL;
			break;
		}
	}

	tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
	{
		if (mostraArvore)
			PrintNodo("^", MANTEM);
		tk.processado = TRUE;
		newSimb.ponteiro = TRUE;
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat != ID)
	{
		error("identificador esperado");
	}
	else
	{
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		tk.processado = TRUE;

		strcpy(newSimb.lexema, tk.lexema);

		int checkSimb = findSimb(newSimb.lexema);
		if (checkSimb)
		{
			if (compEsc(escopo_atual, checkSimb))
			{
				error("Redeclaracao de identificador");
			}
		}
		else
		{
			newSimb.escopo = escopo_atual;
			newSimb.tipo = newSimbType;
			newSimbIdx = insertSimb(newSimb);
		}
	}

	tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_PAR)
	{
		if (mostraArvore)
			PrintNodo("(", MANTEM);
		tk.processado = TRUE;

		TiposParam(newSimbIdx);

		if (tk.processado)
			tk = AnaLex(fd);

		if (tk.cat == SN && tk.codigo == FECHA_PAR)
		{
			if (mostraArvore)
				PrintNodo(")", MANTEM);
			tk.processado = TRUE;
			escopo_atual = GLOBAL;
		}
		else
		{
			error("fechamento de parenteses esperado");
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat != SN && tk.codigo != PONTO_VIRG)
		{
			error("ponto e virgula esperado");
		}
		else
		{
			if (mostraArvore)
				PrintNodo(";", MANTEM);
			tk.processado = TRUE;
		}
	}
	else
	{
		error("abre parenteses esperado");
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void DeclVarFunc()
{
	SIMBOLO newSimb;
	int newSimbIdx;
	int newSimbType;
	newSimb.ponteiro = FALSE;
	newSimb.array = FALSE;

	int countMem = 0;
	char cmdObj[20];

	if (mostraArvore)
		PrintNodo("<DeclVarFunc>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == PR && tk.codigo == VOID)
	{
		if (mostraArvore)
			PrintNodo("VOID", MANTEM);
		tk.processado = TRUE;
		newSimb.tipo = VOID;
	}
	else
	{
		switch (Tipo())
		{
		case 'i':
			newSimbType = INT;
			break;
		case 'f':
			newSimbType = FLOAT;
			break;
		case 'c':
			newSimbType = CHAR;
			break;
		case 'b':
			newSimbType = BOOL;
			break;
		}
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
	{
		newSimb.ponteiro = TRUE;
		if (mostraArvore)
			PrintNodo("^", MANTEM);
		tk.processado = TRUE;
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == ID)
	{
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		tk.processado = TRUE;
		strcpy(newSimb.lexema, tk.lexema);

		int checkSimb = findSimb(tk.lexema);
		if (checkSimb)
		{
			if (compEsc(escopo_atual, checkSimb) && compVarInFunc(newSimb.lexema))
			{
				error("Redeclaracao de identificador");
			}
		}
		else
		{
			newSimb.escopo = escopo_atual;
			newSimb.tipo = newSimbType;
			newSimbIdx = insertSimb(newSimb);
			countMem++;
		}
	}
	else
	{
		error("identificador esperado");
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == PONTO_VIRG)
	{
		if (mostraArvore)
			PrintNodo(";", MANTEM);
		tk.processado = TRUE;

		setPapel(newSimbIdx, VAR);

		if (newSimb.tipo == VOID && !newSimb.ponteiro)
		{
			error("ERRO: Apenas ponteiros podem ter tipo VOID");
		}

		sprintf(cmdObj, "AMEM %d\n", countMem);
		fputs(cmdObj, fd_out);
	}
	else if (tk.cat == SN && tk.codigo == VIRG)
	{
		setPapel(newSimbIdx, VAR);

		countMem += DeclListVar(newSimbType);

		sprintf(cmdObj, "AMEM %d\n", countMem);
		fputs(cmdObj, fd_out);

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat != SN && tk.codigo != PONTO_VIRG)
		{
			error("ponto e virgula esperado");
		}
		else
		{
			if (mostraArvore)
				PrintNodo(";", MANTEM);
			tk.processado = TRUE;
		}
	}
	else if (tk.cat == SN && tk.codigo == ABRE_COL)
	{
		DeclArrayVar(newSimbIdx);

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == VIRG)
		{
			DeclListVar(newSimbType);
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat != SN && tk.codigo != PONTO_VIRG)
		{
			error("ponto e virgula esperado");
		}
		else
		{
			if (mostraArvore)
				PrintNodo(";", MANTEM);
			tk.processado = TRUE;
		}
	}
	else if (tk.cat == SN && tk.codigo == ABRE_PAR)
	{
		FuncProt(newSimbIdx);
	}
	else if (tk.cat == SN && tk.codigo == QUATR_PONTOS)
	{
		if (mostraArvore)
			PrintNodo("::", MANTEM);
		tk.processado = TRUE;

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);
			tk.processado = TRUE;
			// verificar de id de funcao pertence ao escopo (identificador antes dos quatro pontos)
		}
		else
		{
			error("identificador esperado");
		}

		FuncProt(newSimbIdx);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

int DeclListVar(int listSimbType)
{
	int countVars = 0;

	if (mostraArvore)
		PrintNodo("<DeclListVar>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	while (tk.cat == SN && tk.codigo == VIRG)
	{
		if (mostraArvore)
			PrintNodo(",", MANTEM);
		tk.processado = TRUE;

		SIMBOLO newSimb;
		int newSimbIdx;
		newSimb.ponteiro = FALSE;
		newSimb.array = FALSE;
		newSimb.tipo = listSimbType;

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
		{
			if (mostraArvore)
				PrintNodo("^", MANTEM);
			tk.processado = TRUE;
			newSimb.ponteiro = TRUE;
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			countVars++;

			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);
			tk.processado = TRUE;

			strcpy(newSimb.lexema, tk.lexema);

			int checkSimb = findSimb(tk.lexema);
			int checkType = findType(tk.lexema);
			if (checkSimb)
			{
				if (compEsc(escopo_atual, checkSimb) && compVarInFunc(newSimb.lexema))
				{
					error("Redeclaracao de variavel");
				}
				else
				{
					newSimb.escopo = escopo_atual;
					newSimb.papel = VAR;
					PrintNodo(newSimb.lexema, MANTEM);
					newSimbIdx = insertSimb(newSimb);
				}
			}
			else if (checkType)
			{
				error("Redeclaracao de OBJ");
			}
		}
		else
		{
			error("identificador esperado");
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == ABRE_COL)
		{
			DeclArrayVar(newSimbIdx);
		}

		if (tk.processado)
			tk = AnaLex(fd);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);

	return countVars;
}

void DeclArrayVar(int varIdx)
{
	if (mostraArvore)
		PrintNodo("<DeclArrayVar>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_COL)
	{
		if (mostraArvore)
			PrintNodo("[", MANTEM);
		tk.processado = TRUE;
		setIsArray(varIdx);

		tk = AnaLex(fd);
		if (tk.cat != CT_I)
		{
			error("constante inteira esperado");
		}
		else
		{
			if (mostraArvore)
				PrintNodoInt(tk.valInt, MANTEM);
			tk.processado = TRUE;
		}

		tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == FECHA_COL)
		{
			if (mostraArvore)
				PrintNodo("]", MANTEM);
			tk.processado = TRUE;
		}
		else
		{
			error("fechamento de colchetes esperado");
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void MethSec()
{
	if (mostraArvore)
		PrintNodo("<MethSec>", AVANCA);

	// PR CODE
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == PR && tk.codigo == CODE)
	{
		if (mostraArvore)
			PrintNodo("CODE", MANTEM);
		tk.processado = TRUE;
	}
	else
	{
		error("palavra reservada 'code' esperado");
	}

	// dois pontos
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == SN && tk.codigo == DOIS_PONTOS)
	{
		if (mostraArvore)
			PrintNodo(":", MANTEM);
		tk.processado = TRUE;
	}
	else
	{
		error("dois pontos esperado");
	}

	// token apos dois pontos
	tk = AnaLex(fd);
	// sequencia de func_prot depois do dois pontos
	while (tk.cat == PR && (tk.codigo == VOID || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL))
	{
		SignFunc();

		// token apos ponto e virgula
		if (tk.processado)
		{
			tk = AnaLex(fd);
		}
	}
	if (tk.cat == ID)
	{
		// tratar tipo_obj
	}

	// PR INTERN
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == PR && tk.codigo == INTERN)
	{
		if (mostraArvore)
			PrintNodo("INTERN", MANTEM);
		tk.processado = TRUE;
	}
	else
	{
		error("palavra reservada 'intern' esperado");
	}

	// dois pontos
	if (tk.processado)
	{
		tk = AnaLex(fd);
	}
	if (tk.cat == SN && tk.codigo == DOIS_PONTOS)
	{
		if (mostraArvore)
			PrintNodo(":", MANTEM);
		tk.processado = TRUE;
	}
	else
	{
		error("dois pontos esperado");
	}

	// token apos dois pontos
	tk = AnaLex(fd);
	// sequencia de func_prot depois do dois pontos
	while (tk.cat == PR && (tk.codigo == VOID || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL))
	{
		SignFunc();

		// token apos ponto e virgula
		if (tk.processado)
		{
			tk = AnaLex(fd);
		}
	}
	if (tk.cat == ID)
	{
		// tratar tipo_obj
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void FuncProt(int funcIdx)
{
	char cmdObj[20];
	int labelfunc;

	setPapel(funcIdx, FUNC);

	if (mostraArvore)
		PrintNodo("<FuncProt>", AVANCA);

	sprintf(cmdObj, "GOTO L%d\n", countLabels);
	fputs(cmdObj, fd_out);
	labelfunc = countLabels;
	countLabels++;

	sprintf(cmdObj, "LABEL L%d\n", countLabels);
	fputs(cmdObj, fd_out);
	setEndRelativo(funcIdx, countLabels);
	countLabels++;

	fputs("INIPR 1\n", fd_out);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_PAR)
	{
		if (mostraArvore)
			PrintNodo("(", MANTEM);
		tk.processado = TRUE;

		TiposParam(funcIdx);

		if (tk.processado)
			tk = AnaLex(fd);

		if (tk.cat == SN && tk.codigo == FECHA_PAR)
		{
			if (mostraArvore)
				PrintNodo(")", MANTEM);
			tk.processado = TRUE;
			escopo_atual = GLOBAL;
		}
		else
		{
			error("fechamento de parenteses esperado");
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == PONTO_VIRG)
		{
			if (mostraArvore)
				PrintNodo(";", MANTEM);
			tk.processado = TRUE;
		}
		else if (tk.cat == SN && tk.codigo == ABRE_CHAVE)
		{
			Func();
		}
		else if (tk.cat == SN && tk.codigo == VIRG)
		{
			DeclListFunc(funcIdx);

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat != SN && tk.codigo != PONTO_VIRG)
			{
				error("ponto e virgula esperado");
			}
			else
			{
				if (mostraArvore)
					PrintNodo(";", MANTEM);
				tk.processado = TRUE;
			}
		}
		else
		{
			error("virgula, ponto e virgula ou abertura de chaves esperado");
		}
	}
	else
	{
		error("abre parenteses esperado");
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);

	int locals = countLocals(funcIdx);
	if (locals)
	{
		/* code */
	}
	sprintf(cmdObj, "DMEM %d\n", locals);
	fputs(cmdObj, fd_out);

	sprintf(cmdObj, "RET  1,%d\n", countArgs(funcIdx));
	fputs(cmdObj, fd_out);

	sprintf(cmdObj, "LABEL L%d\n", labelfunc);
	fputs(cmdObj, fd_out);
}

void DeclListFunc(int funcIdx)
{
	if (mostraArvore)
		PrintNodo("<DeclListFunc>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	while (tk.cat == SN && tk.codigo == VIRG)
	{
		SIMBOLO newSimb;
		int newSimbIdx;
		int newSimbType = getFuncType(funcIdx);
		newSimb.ponteiro = FALSE;
		newSimb.array = FALSE;
		newSimb.papel = FUNC;

		if (mostraArvore)
			PrintNodo(",", MANTEM);
		tk.processado = TRUE;

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
		{
			if (mostraArvore)
				PrintNodo("^", MANTEM);
			tk.processado = TRUE;
			newSimb.ponteiro = TRUE;
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			strcpy(newSimb.lexema, tk.lexema);

			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);
			tk.processado = TRUE;

			int checkSimb = findSimb(tk.lexema);
			int checkType = findType(tk.lexema);
			if (checkSimb)
			{
				if (compEsc(escopo_atual, checkSimb))
				{
					error("Redeclaracao de identificador");
				}
			}
			else
			{
				newSimb.escopo = escopo_atual;
				newSimb.tipo = newSimbType;
				newSimbIdx = insertSimb(newSimb);
			}
		}
		else
		{
			error("identificador esperado");
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == ABRE_PAR)
		{
			if (mostraArvore)
				PrintNodo("(", MANTEM);
			tk.processado = TRUE;

			TiposParam(newSimbIdx);

			if (tk.processado)
				tk = AnaLex(fd);

			if (tk.cat == SN && tk.codigo == FECHA_PAR)
			{
				if (mostraArvore)
					PrintNodo(")", MANTEM);
				tk.processado = TRUE;
				escopo_atual = GLOBAL;
			}
			else
			{
				error("fechamento de parenteses esperado");
			}

			if (tk.processado)
				tk = AnaLex(fd);
		}
		else
		{
			error("fechamento de parentes esperado");
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Func()
{
	if (mostraArvore)
		PrintNodo("<Func>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_CHAVE)
	{
		if (mostraArvore)
			PrintNodo("{", MANTEM);
		tk.processado = TRUE;
		escopo_atual = LOCAL;

		if (tk.processado)
			tk = AnaLex(fd);
		while (tk.cat == PR && (tk.codigo == VOID || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL))
		{
			DeclVarFunc();
			if (tk.cat != SN && tk.codigo != PONTO_VIRG)
			{
				error("ponto e virgula esperado");
			}
			else
			{
				if (mostraArvore)
					PrintNodo(";", MANTEM);
				tk.processado = TRUE;
			}

			// token apos ponto e virgula
			if (tk.processado)
			{
				tk = AnaLex(fd);
			}
		}

		if (tk.processado)
			tk = AnaLex(fd);
		while (tk.cat != SN && tk.codigo != FECHA_CHAVE)
		{
			Cmd();

			if (tk.processado)
				tk = AnaLex(fd);
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == FECHA_CHAVE)
		{
			if (mostraArvore)
				PrintNodo("}", MANTEM);
			tk.processado = TRUE;
			escopo_atual = GLOBAL;
		}
		else
		{
			error("fechamento de chaves esperado");
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Cmd()
{
	if (mostraArvore)
		PrintNodo("<Cmd>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == PONTO_VIRG)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(";", MANTEM);
	}
	else if (tk.cat == PR)
	{
		if (tk.codigo == DELETE)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("DELETE", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat == ID)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(tk.lexema, MANTEM);

				int checkSimb = findSimb(tk.lexema);
				if (checkSimb)
				{
					if (!SIMB[checkSimb].ponteiro)
					{
						error("ERRO: DELETE utilizado sobre nao ponteiro");
					}
				}
			}
			else
			{
				error("identificador esperado");
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == PONTO_VIRG)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(";", MANTEM);
			}
			else
			{
				error("ponto e virgula esperado");
			}
		}
		else if (tk.codigo == RETURN)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("RETURN", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat != SN && tk.codigo != PONTO_VIRG)
			{
				Expr();
			}
			else if (tk.cat == SN && tk.codigo == PONTO_VIRG)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(";", MANTEM);
			}
			else
			{
				error("ponto e virgula esperado");
			}

			fputs("STOR 1,-5\n", fd_out);
		}
		else if (tk.codigo == FOR)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("FOR", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == ABRE_PAR)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo("(", MANTEM);

				tk = AnaLex(fd);
				if (tk.cat != SN && tk.codigo != PONTO_VIRG)
				{
					Atrib();
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == PONTO_VIRG)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(";", MANTEM);
				}
				else
				{
					error("ponto e virgula esperado");
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat != SN && tk.codigo != PONTO_VIRG)
				{
					Expr();
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == PONTO_VIRG)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(";", MANTEM);
				}
				else
				{
					error("ponto e virgula esperado");
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat != SN && tk.codigo != FECHA_PAR)
				{
					Atrib();
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == FECHA_PAR)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(")", MANTEM);
				}
				else
				{
					error("fecha parenteses esperado");
				}

				Cmd();
			}
			else
			{
				error("abre parenteses esperado");
			}
		}
		else if (tk.codigo == WHILE)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("WHILE", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == ABRE_PAR)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo("(", MANTEM);

				Expr();

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == FECHA_PAR)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(")", MANTEM);
				}
				else
				{
					error("fecha parenteses esperado");
				}

				Cmd();
			}
			else
			{
				error("abre parenteses esperado");
			}
		}
		else if (tk.codigo == IF)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("IF", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == ABRE_PAR)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo("(", MANTEM);

				Expr();

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == FECHA_PAR)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(")", MANTEM);
				}
				else
				{
					error("fecha parenteses esperado");
				}

				Cmd();

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == PR && tk.codigo == ELSE)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo("ELSE", MANTEM);

					Cmd();
				}
			}
			else
			{
				error("abre parenteses esperado");
			}
		}
	}
	else if (tk.cat == SN && tk.codigo == ABRE_CHAVE)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("{", MANTEM);
		escopo_atual = LOCAL;

		tk = AnaLex(fd);
		while ((tk.cat != SN && tk.codigo != FECHA_CHAVE) || (tk.cat == SN && tk.codigo == PONTO_VIRG))
		{
			Cmd();

			if (tk.processado)
				tk = AnaLex(fd);
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == FECHA_CHAVE)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("}", MANTEM);
			escopo_atual = GLOBAL;
		}
		else
		{
			error("fechamento de chave esperado");
		}
	}
	else if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("^", MANTEM);

		tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);

			int checkSimb = findSimb(tk.lexema);
			int checkType = findType(tk.lexema);
			if (checkSimb)
			{
				if (compEsc(escopo_atual, checkSimb))
				{
					error("Redeclaracao de identificador");
				}
			}
			else
			{
				// insertSimb(tk.lexema);
			}

			tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == PONTO)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(".", MANTEM);

				tk = AnaLex(fd);
				if (tk.cat == ID)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(tk.lexema, MANTEM);
					// verificar identificador pertencente a classe

					tk = AnaLex(fd);
					if (tk.cat == SN && tk.codigo == ABRE_PAR)
					{
						tk.processado = TRUE;
						if (mostraArvore)
							PrintNodo("(", MANTEM);

						tk = AnaLex(fd);
						if (tk.cat != SN || tk.codigo != FECHA_PAR)
						{
							Expr();

							if (tk.processado)
								tk = AnaLex(fd);
							while (tk.cat == SN && tk.codigo == VIRG)
							{
								tk.processado = TRUE;
								if (mostraArvore)
									PrintNodo(",", MANTEM);

								Expr();

								if (tk.processado)
									tk = AnaLex(fd);
							}
						}

						if (tk.processado)
							tk = AnaLex(fd);
						if (tk.cat == SN && tk.codigo == FECHA_PAR)
						{
							tk.processado = TRUE;
							if (mostraArvore)
								PrintNodo(")", MANTEM);
						}
						else
						{
							error("fechamento de parenteses esperado");
						}

						if (tk.processado)
							tk = AnaLex(fd);
						if (tk.cat == SN && tk.codigo == PONTO_VIRG)
						{
							tk.processado = TRUE;
							if (mostraArvore)
								PrintNodo(";", MANTEM);
						}
						else
						{
							error("ponto e virgula esperado");
						}
					}
				}
				else
				{
					error("identificador esperado");
				}
			}
			else
			{
				error("ponto esperado");
			}
		}
		else
		{
			error("identificador esperado");
		}
	}
	else if (tk.cat == ID)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);

		int checkSimb = findSimb(tk.lexema);
		if (!checkSimb)
		{
			error("identificador não declarado");
		}

		tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == PONTO)
		{
			int checkType = findType(tk.lexema);
			if (!checkType)
			{
				error("identificador de OBJ não declarado");
			}

			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo(".", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat == ID)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(tk.lexema, MANTEM);
				// verificar identificador de função pertencente a classe

				tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == ABRE_PAR)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo("(", MANTEM);

					tk = AnaLex(fd);
					if (tk.cat != SN || tk.codigo != FECHA_PAR)
					{
						Expr();

						if (tk.processado)
							tk = AnaLex(fd);
						while (tk.cat == SN && tk.codigo == VIRG)
						{
							tk.processado = TRUE;
							if (mostraArvore)
								PrintNodo(",", MANTEM);

							Expr();

							if (tk.processado)
								tk = AnaLex(fd);
						}
					}

					if (tk.processado)
						tk = AnaLex(fd);
					if (tk.cat == SN && tk.codigo == FECHA_PAR)
					{
						tk.processado = TRUE;
						if (mostraArvore)
							PrintNodo(")", MANTEM);
					}
					else
					{
						error("fechamento de parenteses esperado");
					}

					if (tk.processado)
						tk = AnaLex(fd);
					if (tk.cat == SN && tk.codigo == PONTO_VIRG)
					{
						tk.processado = TRUE;
						if (mostraArvore)
							PrintNodo(";", MANTEM);
					}
					else
					{
						error("ponto e virgula esperado");
					}
				}
			}
			else
			{
				error("identificador esperado");
			}
		}
		else if (tk.cat == SN && tk.codigo == ABRE_PAR)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("(", MANTEM);

			tk = AnaLex(fd);
			if (tk.cat != SN || tk.codigo != FECHA_PAR)
			{
				Expr();

				if (tk.processado)
					tk = AnaLex(fd);
				while (tk.cat == SN && tk.codigo == VIRG)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(",", MANTEM);

					Expr();

					if (tk.processado)
						tk = AnaLex(fd);
				}
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == FECHA_PAR)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(")", MANTEM);
			}
			else
			{
				error("fechamento de parenteses esperado");
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == PONTO_VIRG)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(";", MANTEM);
			}
			else
			{
				error("ponto e virgula esperado");
			}
		}
		else
		{
			RestAtrib(checkSimb);

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == PONTO_VIRG)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(";", MANTEM);
			}
			else
			{
				error("ponto e virgula esperado");
			}
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void RestAtrib(int simbIdx)
{
	char cmdObj[20];

	if (mostraArvore)
		PrintNodo("<RestAtrib>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_COL)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("[", MANTEM);

		Expr();

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.codigo == FECHA_COL)
		{
			if (mostraArvore)
				PrintNodo("]", MANTEM);
			tk.processado = TRUE;
		}
		else
		{
			error("Fechamento de Colchetes esperado!");
		}
	}

	if (tk.cat != OP_ARIT || tk.codigo != ATRIB)
	{
		error("Sinal de atribuicao esperado!");
	}

	if (mostraArvore)
		PrintNodo("=", MANTEM);
	tk.processado = TRUE;

	tk = AnaLex(fd);
	if (tk.cat == PR && tk.codigo == NEW)
	{
		if (mostraArvore)
			PrintNodo("new", MANTEM);
		tk.processado = TRUE;
		Tipo();
	}
	else
	{
		Expr();
	}

	sprintf(cmdObj, "STOR %d,%d\n", getEscopo(simbIdx), getEndRelativo(simbIdx));
	fputs(cmdObj, fd_out);

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void TiposParam(int funcIdx)
{
	escopo_atual = LOCAL;

	SIMBOLO newSimb;
	int newSimbIdx;
	int newSimbType;
	newSimb.ponteiro = FALSE;
	newSimb.array = FALSE;
	newSimb.papel = PARAM;

	if (mostraArvore)
		PrintNodo("<TiposParam>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);

	if (tk.cat != SN && tk.codigo != FECHA_PAR)
	{
		if (tk.cat == PR && tk.codigo == VOID)
		{
			if (mostraArvore)
				PrintNodo("VOID", MANTEM);
			tk.processado = TRUE;
			newSimbType = VOID;
		}
		else
		{
			switch (Tipo())
			{
			case 'i':
				newSimbType = INT;
				break;
			case 'f':
				newSimbType = FLOAT;
				break;
			case 'c':
				newSimbType = CHAR;
				break;
			case 'b':
				newSimbType = BOOL;
				break;
			}
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == E_COMERC)
		{
			if (mostraArvore)
				PrintNodo("&", MANTEM);
			tk.processado = TRUE;
			newSimb.referencia = TRUE;

			tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
			{
				if (mostraArvore)
					PrintNodo("^", MANTEM);
				tk.processado = TRUE;
				newSimb.ponteiro = TRUE;
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == ID)
			{
				if (mostraArvore)
					PrintNodo(tk.lexema, MANTEM);
				tk.processado = TRUE;
				strcpy(newSimb.lexema, tk.lexema);

				int checkSimb = findSimb(tk.lexema);
				if (checkSimb)
				{
					if (compEsc(escopo_atual, checkSimb) && compFunc(checkSimb, funcIdx))
					{
						error("Redeclaracao de identificador");
					}
				}
				else
				{
					newSimb.escopo = escopo_atual;
					newSimbIdx = insertSimb(newSimb);
				}
			}
			else
			{
				error("identificador esperado");
			}
		}
		else
		{
			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
			{
				if (mostraArvore)
					PrintNodo("^", MANTEM);
				tk.processado = TRUE;
				newSimb.ponteiro = TRUE;
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == ID)
			{
				if (mostraArvore)
					PrintNodo(tk.lexema, MANTEM);
				tk.processado = TRUE;
				strcpy(newSimb.lexema, tk.lexema);

				int checkSimb = findSimb(tk.lexema);
				if (checkSimb)
				{
					if (compEsc(escopo_atual, checkSimb) && compFunc(checkSimb, funcIdx))
					{
						error("Redeclaracao de identificador");
					}
				}
				else
				{
					newSimb.tipo = newSimbType;
					newSimb.escopo = escopo_atual;
					newSimbIdx = insertSimb(newSimb);
				}
			}
			else
			{
				error("identificador esperado");
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == ABRE_COL)
			{
				setIsArray(newSimbIdx);
				if (mostraArvore)
					PrintNodo("[", MANTEM);
				tk.processado = TRUE;

				tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == FECHA_COL)
				{
					if (mostraArvore)
						PrintNodo("]", MANTEM);
					tk.processado = TRUE;
				}
				else
				{
					error("fechamento de colchetes esperado");
				}
			}
		}

		// ocorrencia de virgula indicando declaracao de mais parametros
		if (tk.processado)
			tk = AnaLex(fd);
		while (tk.cat == SN && tk.codigo == VIRG)
		{
			SIMBOLO newSimb;
			int newSimbIdx;
			int newSimbType;
			newSimb.ponteiro = FALSE;
			newSimb.array = FALSE;
			newSimb.papel = PARAM;

			if (mostraArvore)
				PrintNodo(",", MANTEM);
			tk.processado = TRUE;

			if (tk.cat == PR && tk.codigo == VOID)
			{
				if (mostraArvore)
					PrintNodo("VOID", MANTEM);
				tk.processado = TRUE;
				newSimbType = VOID;
			}
			else
			{
				switch (Tipo())
				{
				case 'i':
					newSimbType = INT;
					break;
				case 'f':
					newSimbType = FLOAT;
					break;
				case 'c':
					newSimbType = CHAR;
					break;
				case 'b':
					newSimbType = BOOL;
					break;
				}
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == E_COMERC)
			{
				if (mostraArvore)
					PrintNodo("&", MANTEM);
				tk.processado = TRUE;
				newSimb.referencia = TRUE;

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
				{
					if (mostraArvore)
						PrintNodo("^", MANTEM);
					tk.processado = TRUE;
					newSimb.ponteiro = TRUE;
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat != ID)
				{
					error("identificador esperado");
				}
				else
				{
					if (mostraArvore)
						PrintNodo(tk.lexema, MANTEM);
					tk.processado = TRUE;

					strcpy(newSimb.lexema, tk.lexema);

					int checkSimb = findSimb(tk.lexema);
					if (checkSimb)
					{
						if (compEsc(escopo_atual, checkSimb))
						{
							error("Redeclaracao de identificador");
						}
					}
					else
					{
						newSimb.tipo = newSimbType;
						newSimb.escopo = escopo_atual;
						newSimbIdx = insertSimb(newSimb);
					}
				}
			}
			else
			{
				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
				{
					if (mostraArvore)
						PrintNodo("^", MANTEM);
					tk.processado = TRUE;
					newSimb.ponteiro = TRUE;
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat != ID)
				{
					error("identificador esperado");
				}
				else
				{
					if (mostraArvore)
						PrintNodo(tk.lexema, MANTEM);
					tk.processado = TRUE;

					strcpy(newSimb.lexema, tk.lexema);

					int checkSimb = findSimb(tk.lexema);
					if (checkSimb)
					{
						if (compEsc(escopo_atual, checkSimb))
						{
							error("Redeclaracao de identificador");
						}
					}
					else
					{
						newSimb.tipo = newSimbType;
						newSimb.escopo = escopo_atual;
						newSimbIdx = insertSimb(newSimb);
					}
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == ABRE_COL)
				{
					setIsArray(newSimbIdx);
					if (mostraArvore)
						PrintNodo("[", MANTEM);
					tk.processado = TRUE;

					tk = AnaLex(fd);
					if (tk.cat == SN && tk.codigo == FECHA_COL)
					{
						if (mostraArvore)
							PrintNodo("]", MANTEM);
						tk.processado = TRUE;
					}
					else
					{
						error("fechamento de colchetes esperado");
					}
				}
			}

			if (tk.processado)
				tk = AnaLex(fd);
		}
	}
	else if (mostraArvore)
	{
		PrintNodo("<e>", MANTEM);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Atrib()
{
	int simb;
	char cmdObj[20];

	if (mostraArvore)
		PrintNodo("<Atrib>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == ID)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		simb = findSimb(tk.lexema);

		if (!simb)
		{
			error("identificador nao declarado");
		}
	}
	else
	{
		error("identificador esperado");
	}

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_COL)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("[", MANTEM);

		Expr();

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.codigo == FECHA_COL)
		{
			if (mostraArvore)
				PrintNodo("]", MANTEM);
			tk.processado = TRUE;
		}
		else
		{
			error("Fechamento de Colchetes esperado!");
		}
	}

	if (tk.cat != OP_ARIT || tk.codigo != ATRIB)
	{
		error("Sinal de atribuicao esperado!");
	}
	if (mostraArvore)
		PrintNodo("=", MANTEM);
	tk.processado = TRUE;

	tk = AnaLex(fd);
	if (tk.cat == PR && tk.codigo == NEW)
	{
		if (mostraArvore)
			PrintNodo("new", MANTEM);
		tk.processado = TRUE;
		Tipo();
	}
	else
	{
		Expr();
	}

	sprintf(cmdObj, "STOR %d,%d\n", getEscopo(simb), getEndRelativo(simb));
	fputs(cmdObj, fd_out);

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

char Tipo()
{
	if (mostraArvore)
		PrintNodo("<Tipo>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);

	if (tk.cat == PR)
	{
		if (tk.codigo == INT)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("INT", MANTEM);
			return 'i';
		}
		else if (tk.codigo == CHAR)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("CHAR", MANTEM);
			return 'c';
		}
		else if (tk.codigo == FLOAT)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("FLOAT", MANTEM);
			return 'f';
		}
		else if (tk.codigo == BOOL)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("BOOL", MANTEM);
			return 'b';
		}
	}
	else if (tk.cat == ID)
	{
		// verificar ID da tabela de tipos
		int typeidx = findType(tk.lexema);
		if (typeidx)
		{
			return typeidx;
		}
		else
		{
			error("Identificador nao corresponde a um tipo valido!");
		}
	}
	else
	{
		error("Identificador de tipo esperado!");
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Expr()
{

	if (mostraArvore)
		PrintNodo("<Expr>", AVANCA);

	ExprSimp();

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == OP_LOGIC)
	{
		OpRel();
		ExprSimp();
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void ExprSimp()
{
	int temOu = FALSE;
	char cmdObj[20];
	int operacao;

	if (mostraArvore)
		PrintNodo("<ExprSimp>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == OP_ARIT && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO))
	{
		tk.processado = TRUE;
		operacao = tk.codigo;
		if (mostraArvore)
		{
			if (tk.codigo == ADICAO)
				PrintNodo("+", MANTEM);
			else
				PrintNodo("-", MANTEM);
		}
	}

	Termo();

	if (operacao == ADICAO)
		fputs("ADD\n", fd_out);
	else if (operacao == SUBTRACAO)
		fputs("SUB\n", fd_out);

	if (tk.processado)
		tk = AnaLex(fd);
	while ((tk.cat == OP_ARIT && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO)) || (tk.cat == OP_LOGIC && tk.codigo == OU_LOGIC))
	{
		tk.processado = TRUE;
		operacao = tk.codigo;
		if (mostraArvore)
		{
			if (tk.codigo == ADICAO)
				PrintNodo("+", MANTEM);
			else if (tk.codigo == SUBTRACAO)
				PrintNodo("-", MANTEM);
			else
			{
				PrintNodo("||", MANTEM);
				temOu = TRUE;
			}
		}

		Termo();

		if (operacao == ADICAO)
			fputs("ADD\n", fd_out);
		else if (operacao == SUBTRACAO)
			fputs("SUB\n", fd_out);
		else
		{
			sprintf(cmdObj, "COPY \n");
			sprintf(cmdObj, "GOTRUE L%d\n", countLabels);
			sprintf(cmdObj, "POP \n");
			fputs(cmdObj, fd_out);
		}

		if (tk.processado)
			tk = AnaLex(fd);
	}
	if (temOu)
	{
		sprintf(cmdObj, "LABEL L%d\n", countLabels);
		fputs(cmdObj, fd_out);
		countLabels++;
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void OpRel()
{
	if (mostraArvore)
		PrintNodo("<OpRel>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.codigo == IGUAL_A)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("==", MANTEM);
	}
	else if (tk.codigo == DIFERENTE)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("!=", MANTEM);
	}
	else if (tk.codigo == MAIOR_QUE)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(">", MANTEM);
	}
	else if (tk.codigo == MENOR_QUE)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("<", MANTEM);
	}
	else if (tk.codigo == MAIOR_IGUAL)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(">=", MANTEM);
	}
	else if (tk.codigo == MENOR_IGUAL)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("<=", MANTEM);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Termo()
{
	int temE = FALSE;
	char cmdObj[20];
	int operacao;

	if (mostraArvore)
		PrintNodo("<Termo>", AVANCA);

	Fator();

	if (tk.processado)
		tk = AnaLex(fd);
	while ((tk.cat == OP_ARIT && (tk.codigo == MULTIPLIC || tk.codigo == DIVISAO)) || (tk.cat == OP_LOGIC && tk.codigo == E_LOGIC))
	{
		tk.processado = TRUE;
		operacao = tk.codigo;
		if (mostraArvore)
		{
			if (tk.codigo == MULTIPLIC)
				PrintNodo("*", MANTEM);
			else if (tk.codigo == DIVISAO)
				PrintNodo("/", MANTEM);
			else
				PrintNodo("&&", MANTEM);
		}

		Fator();

		if (operacao == MULTIPLIC)
			fputs("MUL\n", fd_out);
		else if (operacao == DIVISAO)
			fputs("DIV\n", fd_out);
		else
		{
			sprintf(cmdObj, "COPY \n");
			sprintf(cmdObj, "GOFALSE L%d\n", countLabels);
			sprintf(cmdObj, "POP \n");
			fputs(cmdObj, fd_out);
		}

		if (tk.processado)
			tk = AnaLex(fd);
	}

	if (temE)
	{
		sprintf(cmdObj, "LABEL L%d\n", countLabels);
		fputs(cmdObj, fd_out);
		countLabels++;
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Fator()
{
	char cmdObj[20];

	if (mostraArvore)
		PrintNodo("<Fator>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == CT_I)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.valInt, MANTEM);

		sprintf(cmdObj, "PUSH %d\n", tk.valInt);
		fputs(cmdObj, fd_out);
	}
	else if (tk.cat == CT_F)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.valFloat, MANTEM);

		sprintf(cmdObj, "PUSH %d\n", tk.valFloat);
		fputs(cmdObj, fd_out);
	}
	else if (tk.cat == CT_C)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.caracter, MANTEM);

		sprintf(cmdObj, "PUSH %d\n", tk.caracter);
		fputs(cmdObj, fd_out);
	}
	else if (tk.cat == SN && tk.codigo == ABRE_PAR)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("(", MANTEM);

		Expr();

		if (tk.cat != SN || tk.codigo != FECHA_PAR)
		{
			error("Fecha parenteses esperado!");
		}
		else
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo(")", MANTEM);
		}
	}
	else if (tk.cat == OP_LOGIC && tk.codigo == NEGACAO)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo("!", MANTEM);

		Fator();
	}
	else
	{
		if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
		{
			if (mostraArvore)
				PrintNodo("^", MANTEM);
			tk.processado = TRUE;
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);

			int checkSimb = findSimb(tk.lexema);
			if (!checkSimb)
			{
				error("Identificador não declarado");
			}

			if (!checkIsFunc(checkSimb))
			{
				sprintf(cmdObj, "LOAD %d,%d\n", getEscopo(checkSimb), getEndRelativo(checkSimb));
				fputs(cmdObj, fd_out);
			}

			tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == PONTO)
			{
				int checkType = findType(tk.lexema);
				if (!checkType)
				{
					error("Identificador não declarado");
				}

				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo(".", MANTEM);

				tk = AnaLex(fd);
				if (tk.cat == ID)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(tk.lexema, MANTEM);

					tk = AnaLex(fd);
					if (tk.cat == SN && tk.codigo == ABRE_PAR)
					{
						tk.processado = TRUE;
						if (mostraArvore)
							PrintNodo("(", MANTEM);

						tk = AnaLex(fd);
						if (tk.cat != SN || tk.codigo != FECHA_PAR)
						{
							Expr();

							if (tk.processado)
								tk = AnaLex(fd);
							while (tk.cat == SN && tk.codigo == VIRG)
							{
								tk.processado = TRUE;
								if (mostraArvore)
									PrintNodo(",", MANTEM);

								Expr();

								if (tk.processado)
									tk = AnaLex(fd);
							}
						}

						if (tk.processado)
							tk = AnaLex(fd);
						if (tk.cat == SN && tk.codigo == FECHA_PAR)
						{
							tk.processado = TRUE;
							if (mostraArvore)
								PrintNodo(")", MANTEM);
						}
						else
						{
							error("fechamento de parenteses esperado");
						}
					}
					else if (tk.cat == SN && tk.codigo == ABRE_COL)
					{
						tk.processado = TRUE;
						if (mostraArvore)
							PrintNodo("[", MANTEM);

						Expr();

						tk = AnaLex(fd);
						if (tk.cat == SN && tk.codigo == FECHA_COL)
						{
							tk.processado = TRUE;
							if (mostraArvore)
								PrintNodo("]", MANTEM);
						}
						else
						{
							error("fechamento de colchetes esperado");
						}
					}
				}
				else
				{
					error("identificador esperado");
				}
			}
			else if (tk.cat == SN && tk.codigo == ABRE_COL)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo("[", MANTEM);

				Expr();

				tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == FECHA_COL)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo("]", MANTEM);
				}
				else
				{
					error("fechamento de colchetes esperado");
				}
			}
			else if (tk.cat == SN && tk.codigo == ABRE_PAR)
			{
				tk.processado = TRUE;
				if (mostraArvore)
					PrintNodo("(", MANTEM);

				tk = AnaLex(fd);
				if (tk.cat != SN || tk.codigo != FECHA_PAR)
				{
					Expr();

					if (tk.processado)
						tk = AnaLex(fd);
					while (tk.cat == SN && tk.codigo == VIRG)
					{
						tk.processado = TRUE;
						if (mostraArvore)
							PrintNodo(",", MANTEM);

						Expr();

						if (tk.processado)
							tk = AnaLex(fd);
					}
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == FECHA_PAR)
				{
					tk.processado = TRUE;
					if (mostraArvore)
						PrintNodo(")", MANTEM);

					sprintf(cmdObj, "CALL L%d\n", getEndRelativo(checkSimb));
					fputs(cmdObj, fd_out);
				}
				else
				{
					error("fechamento de parenteses esperado");
				}
			}
		}
		else
		{
			error("identificador esperado");
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}
