#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnaLex.h"
#include "AnaSint.h"
#include "FuncAux.h"

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

	// tk = AnaLex(fd);
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
		if (tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL)
		{
			tk.processado = TRUE;
			if (mostraArvore)
				PrintNodo("int", MANTEM);
		}
		else
		{
			// tratar tipo_obj e tipo invalido
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
