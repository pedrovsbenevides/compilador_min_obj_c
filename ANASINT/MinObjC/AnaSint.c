#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"
void Prog()
{
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
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		tk.processado = TRUE;
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
		DeclVarFunc();

		// ponto e virgula esperado
		if (tk.processado)
		{
			tk = AnaLex(fd);
		}
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

		// sequencia de declListVar depois do ponto e virgula
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

void DeclVarFunc()
{
	if (mostraArvore)
		PrintNodo("<DeclVarFunc>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == PR && tk.codigo == VOID)
	{
		if (mostraArvore)
			PrintNodo("VOID", MANTEM);
		tk.processado = TRUE;
	}
	else
	{
		Tipo();
	}

	if (tk.processado)
		tk = AnaLex(fd);
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
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		tk.processado = TRUE;
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
	}
	else if (tk.cat == SN && tk.codigo == VIRG)
	{
		DeclListVar();

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
		DeclArrayVar();

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == VIRG)
		{
			DeclListVar();
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
		FuncProt();
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
		}
		else
		{
			error("identificador esperado");
		}

		FuncProt();
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void DeclListVar()
{
	if (mostraArvore)
		PrintNodo("<DeclListVar>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	while (tk.cat == SN && tk.codigo == VIRG)
	{
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
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);
			tk.processado = TRUE;
		}
		else
		{
			error("identificador esperado");
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == ABRE_COL)
		{
			DeclArrayVar();
		}

		if (tk.processado)
			tk = AnaLex(fd);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void DeclArrayVar()
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
		DeclVarFunc();

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
		DeclVarFunc();

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

void FuncProt()
{
	if (mostraArvore)
		PrintNodo("<FuncProt>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == SN && tk.codigo == ABRE_PAR)
	{
		if (mostraArvore)
			PrintNodo("(", MANTEM);
		tk.processado = TRUE;

		TiposParam();

		if (tk.processado)
			tk = AnaLex(fd);

		if (tk.cat == SN && tk.codigo == FECHA_PAR)
		{
			if (mostraArvore)
				PrintNodo(")", MANTEM);
			tk.processado = TRUE;
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
			DeclListFunc();

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
}

void DeclListFunc()
{
	if (mostraArvore)
		PrintNodo("<DeclListFunc>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	while (tk.cat == SN && tk.codigo == VIRG)
	{
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
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == ID)
		{
			if (mostraArvore)
				PrintNodo(tk.lexema, MANTEM);
			tk.processado = TRUE;
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

			TiposParam();

			if (tk.processado)
				tk = AnaLex(fd);

			if (tk.cat == SN && tk.codigo == FECHA_PAR)
			{
				if (mostraArvore)
					PrintNodo(")", MANTEM);
				tk.processado = TRUE;
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

		// while (tk.cat == PR || tk.cat == SN || tk.cat == ID)
		// {
		// 	Cmd();
		// }

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == FECHA_CHAVE)
		{
			if (mostraArvore)
				PrintNodo("}", MANTEM);
			tk.processado = TRUE;
		}
		else
		{
			error("fechamento de chaves esperado");
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void TiposParam()
{
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
		}
		else
		{
			Tipo();
		}

		if (tk.processado)
			tk = AnaLex(fd);
		if (tk.cat == SN && tk.codigo == E_COMERC)
		{
			if (mostraArvore)
				PrintNodo("&", MANTEM);
			tk.processado = TRUE;

			tk = AnaLex(fd);
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
				if (mostraArvore)
					PrintNodo(tk.lexema, MANTEM);
				tk.processado = TRUE;
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
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == ID)
			{
				if (mostraArvore)
					PrintNodo(tk.lexema, MANTEM);
				tk.processado = TRUE;
			}
			else
			{
				error("identificador esperado");
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == ABRE_COL)
			{
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
			if (mostraArvore)
				PrintNodo(",", MANTEM);
			tk.processado = TRUE;

			if (tk.cat == PR && tk.codigo == VOID)
			{
				if (mostraArvore)
					PrintNodo("VOID", MANTEM);
				tk.processado = TRUE;
			}
			else
			{
				Tipo();
			}

			if (tk.processado)
				tk = AnaLex(fd);
			if (tk.cat == SN && tk.codigo == E_COMERC)
			{
				if (mostraArvore)
					PrintNodo("&", MANTEM);
				tk.processado = TRUE;

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == CIRCUNFLEXO)
				{
					if (mostraArvore)
						PrintNodo("^", MANTEM);
					tk.processado = TRUE;
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
				}

				if (tk.processado)
					tk = AnaLex(fd);
				if (tk.cat == SN && tk.codigo == ABRE_COL)
				{
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

	if (mostraArvore)
		PrintNodo("<Atrib>", AVANCA);

	// if (tk.processado)
	tk = AnaLex(fd);
	if (tk.cat == ID)
	{
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
	}
	else
	{
		error("Identificador esperado!");
	}

	tk = AnaLex(fd);
	if (tk.cat == SN)
	{
		if (tk.codigo == ABRE_COL)
		{
			if (mostraArvore)
				PrintNodo("[", MANTEM);
			tk.processado = TRUE;
			Expr();
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
		else
		{
			error("Abertura de Colchetes esperado!");
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

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Tipo()
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
		}
		else if (tk.codigo == CHAR)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("CHAR", MANTEM);
		}
		else if (tk.codigo == FLOAT)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("FLOAT", MANTEM);
		}
		else if (tk.codigo == BOOL)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("BOOL", MANTEM);
		}
	}
	else if (tk.cat == ID)
	{
		// verificar ID da tabela de tipos
		error("Identificador nao corresponde a um tipo valido!");
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
	Termo();
	Resto();
	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Termo()
{

	if (mostraArvore)
		PrintNodo("<Termo>", AVANCA);
	Fator();
	Sobra();
	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Resto()
{

	if (mostraArvore)
		PrintNodo("<Resto>", AVANCA);
	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == OP_ARIT && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO))
	{
		tk.processado = TRUE;
		if (mostraArvore)
		{
			if (tk.codigo == ADICAO)
				PrintNodo("+", MANTEM);
			else
				PrintNodo("-", MANTEM);
		}
		Termo();
		Resto();
	}
	else if (mostraArvore)
		PrintNodo("<e>", MANTEM);

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Sobra()
{

	if (mostraArvore)
		PrintNodo("<Sobra>", AVANCA);
	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == OP_ARIT && (tk.codigo == MULTIPLIC || tk.codigo == DIVISAO))
	{
		tk.processado = TRUE;
		if (mostraArvore)
		{
			if (tk.codigo == MULTIPLIC)
				PrintNodo("*", MANTEM);
			else
				PrintNodo("/", MANTEM);
		}
		Fator();
		Sobra();
	}
	else if (mostraArvore)
		PrintNodo("<e>", MANTEM);

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Fator()
{

	if (mostraArvore)
		PrintNodo("<Fator>", AVANCA);
	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == ID)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
		/*TRATAR IDENTIFICADOR*/
	}
	else if (tk.cat == CT_I)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.valInt, MANTEM);
		/*TRATAR CONSTANTE INTEIRA*/
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
	else
	{
		error("Identificador, constante inteira ou abre parenteses esperado!");
	}
	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}
