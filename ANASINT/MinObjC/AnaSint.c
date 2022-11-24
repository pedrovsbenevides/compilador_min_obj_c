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
		DeclVar();

		// ponto e virgula esperado
		// if (tk.processado)
		// {
		// 	tk = AnaLex(fd);
		// }
		// if (tk.cat != SN && tk.codigo != PONTO_VIRG)
		// {
		// 	error("ponto e virgula esperado");
		// }
		// else
		// {
		// 	if (mostraArvore)
		// 		PrintNodo(";", MANTEM);
		// 	tk.processado = TRUE;
		// }

		// sequencia de declListVar depois do ponto e virgula
		tk = AnaLex(fd);
		while (tk.cat == PR && (tk.codigo == VOID || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == FLOAT || tk.codigo == BOOL))
		{
			DeclVar();
			// if (tk.cat != SN && tk.codigo != PONTO_VIRG)
			// {
			// 	error("ponto e virgula esperado");
			// }
			// else
			// {
			// 	if (mostraArvore)
			// 		PrintNodo(";", MANTEM);
			// 	tk.processado = TRUE;
			// }

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
	if (mostraArvore)
		PrintNodo("<DeclVar>", AVANCA);

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

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void SignFunc()
{
	if (mostraArvore)
		PrintNodo("<SignFunc>", AVANCA);

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

		while (tk.cat != SN && tk.codigo != FECHA_CHAVE)
		{
			Cmd();
		}

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
					Atrib(1);
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
					Atrib(1);
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
			/*TRATAR IDENTIFICADOR*/

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
		else
		{
			RestAtrib();

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

void RestAtrib()
{
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

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == ID)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodo(tk.lexema, MANTEM);
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
	if (mostraArvore)
		PrintNodo("<ExprSimp>", AVANCA);

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
	}

	Termo();

	if (tk.processado)
		tk = AnaLex(fd);
	while ((tk.cat == OP_ARIT && (tk.codigo == ADICAO || tk.codigo == SUBTRACAO)) || (tk.cat == OP_LOGIC && tk.codigo == OU_LOGIC))
	{
		tk.processado = TRUE;
		if (mostraArvore)
		{
			if (tk.codigo == ADICAO)
				PrintNodo("+", MANTEM);
			else if (tk.codigo == SUBTRACAO)
				PrintNodo("-", MANTEM);
			else
				PrintNodo("||", MANTEM);
		}

		Termo();

		if (tk.processado)
			tk = AnaLex(fd);
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

	if (mostraArvore)
		PrintNodo("<Termo>", AVANCA);

	Fator();

	if (tk.processado)
		tk = AnaLex(fd);
	while ((tk.cat == OP_ARIT && (tk.codigo == MULTIPLIC || tk.codigo == DIVISAO)) || (tk.cat == OP_LOGIC && tk.codigo == E_LOGIC))
	{
		tk.processado = TRUE;
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

		if (tk.processado)
			tk = AnaLex(fd);
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}

void Fator()
{

	if (mostraArvore)
		PrintNodo("<Fator>", AVANCA);

	if (tk.processado)
		tk = AnaLex(fd);
	if (tk.cat == CT_I)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.valInt, MANTEM);
		/*TRATAR CONSTANTE INTEIRA*/
	}
	else if (tk.cat == CT_F)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.valFloat, MANTEM);
	}
	else if (tk.cat == CT_C)
	{
		tk.processado = TRUE;
		if (mostraArvore)
			PrintNodoInt(tk.caracter, MANTEM);
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
			/*TRATAR IDENTIFICADOR*/

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
		}
		else
		{
			error("identificador esperado");
		}
	}

	if (mostraArvore)
		PrintNodo("", RETROCEDE);
}
