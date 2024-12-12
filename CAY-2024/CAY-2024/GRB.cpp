#include "stdafx.h"

namespace GRB
{
	Greibach greibach(
		NS('S'),
		TS('$'),
		6,
		Rule(
			NS('S'),
			GRB_ERROR_SERIES + 0,
			38,
			// main (2)
			Rule::Chain(3, TS('m'), TS('{'), TS('}')),
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),

			// Создания, инициализация переменной (4)
			Rule::Chain(4, TS('v'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('v'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(6, TS('v'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('v'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('S')),

			// Условия (4)
			Rule::Chain(7, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(8, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(12, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('S')),
			// Условия доп. для пустых (8)
			Rule::Chain(6, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(7, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(9, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}')),
			// Пустое условие (6)
			Rule::Chain(5, TS('z'), TS('('), TS(')'), TS('{'), TS('}')),
			Rule::Chain(6, TS('z'), TS('('), TS(')'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(6, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(7, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(11, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('S')),

			// Циклы (8)
			Rule::Chain(7, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(8, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(6, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), TS('}')),
			Rule::Chain(7, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(6, TS('w'), TS('('), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(7, TS('w'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(5, TS('w'), TS('('), TS(')'), TS('{'), TS('}')),
			Rule::Chain(6, TS('w'), TS('('), TS(')'), TS('{'), TS('}'), NS('S')),

			// Создание функций (6)
			Rule::Chain(7, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), TS('}')),
			Rule::Chain(8, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(8, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(9, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(9, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S'))
		),
		Rule(
			NS('N'),
			GRB_ERROR_SERIES + 1,
			57,
			// Создания, присваивание и инициализация переменной (10)
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),																			
			Rule::Chain(5, TS('i'), TS('='), NS('E'), NS('M'), TS(';')),																
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),																
			Rule::Chain(6, TS('i'), TS('='), NS('E'), NS('M'), TS(';'), NS('N')),														
			Rule::Chain(4, TS('v'), TS('t'), TS('i'), TS(';')),																			
			Rule::Chain(5, TS('v'), TS('t'), TS('i'), TS(';'), NS('N')),																
			Rule::Chain(6, TS('v'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),														
			Rule::Chain(7, TS('v'), TS('t'), TS('i'), TS('='), NS('E'), NS('M'), TS(';')),												
			Rule::Chain(7, TS('v'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),												
			Rule::Chain(8, TS('v'), TS('t'), TS('i'), TS('='), NS('E'), NS('M'), TS(';'), NS('N')),									
			
			// Функции в переменную (8)
			Rule::Chain(6, TS('i'), TS('='), TS('i'), TS('('), TS(')'), TS(';')),
			Rule::Chain(7, TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
			Rule::Chain(7, TS('i'), TS('='), TS('i'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(8, TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(8, TS('v'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), TS(')'), TS(';')),
			Rule::Chain(9, TS('v'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
			Rule::Chain(9, TS('v'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(10, TS('v'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),

			// Условия (4)
			Rule::Chain(7, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),												
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),			
			Rule::Chain(8, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),										
			Rule::Chain(12, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('N')),
			// Условия доп. для пустых (8)
			Rule::Chain(6, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(7, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(9, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}')),
			// Пустое условие (6)
			Rule::Chain(5, TS('z'), TS('('), TS(')'), TS('{'), TS('}')),
			Rule::Chain(6, TS('z'), TS('('), TS(')'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(6, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(7, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(11, TS('z'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('S')),

			// Циклы (8)
			Rule::Chain(7, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),												
			Rule::Chain(8, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(6, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), TS('}')),
			Rule::Chain(7, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(6, TS('w'), TS('('), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(7, TS('w'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(5, TS('w'), TS('('), TS(')'), TS('{'), TS('}')),
			Rule::Chain(6, TS('w'), TS('('), TS(')'), TS('{'), TS('}'), NS('N')),

			// Возвращение занчения из функции (1)
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),

			// Вызов функций (4)
			Rule::Chain(4, TS('i'), TS('('), TS(')'), TS(';')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),

			// write / writeline (8)
			Rule::Chain(6, TS('p'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('p'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(6, TS('s'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('s'), TS('('), NS('E'), TS(')'), TS(';')),
			Rule::Chain(5, TS('p'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(4, TS('p'), TS('('), TS(')'), TS(';')),
			Rule::Chain(5, TS('s'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(4, TS('s'), TS('('), TS(')'), TS(';'))
		),
		Rule(
			NS('E'),
			GRB_ERROR_SERIES + 2,
			10,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(
			NS('M'),
			GRB_ERROR_SERIES + 3,
			16,
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(2, TS('/'), NS('E')),
			Rule::Chain(2, TS('%'), NS('E')),
			Rule::Chain(2, TS('>'), NS('E')),
			Rule::Chain(2, TS('<'), NS('E')),
			Rule::Chain(2, TS('&'), NS('E')),
			Rule::Chain(2, TS('j'), NS('E')),
			Rule::Chain(2, TS('x'), NS('E')),
			Rule::Chain(2, TS('k'), NS('E')),
			Rule::Chain(3, TS('+'), NS('E'), NS('M')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),
			Rule::Chain(3, TS('*'), NS('E'), NS('M')),
			Rule::Chain(3, TS('/'), NS('E'), NS('M')),
			Rule::Chain(3, TS('%'), NS('E'), NS('M'))
		),
		Rule(
			NS('F'),
			GRB_ERROR_SERIES + 4,
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'),
			GRB_ERROR_SERIES + 5,
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		(*this).nt = new GRBALPHABET[(*this).size = psize];
		int* ptr = (int*)&s;
		for (int i = 0; i < psize; i++)
		{
			(*this).nt[i] = (short)ptr[i];
		}
	}

	Rule::Rule(GRBALPHABET pnn, int iderroe, short psize, Chain c, ...)
	{
		(*this).nn = pnn;
		(*this).iderror = iderroe;
		(*this).chains = new Chain[(*this).size = psize];
		Chain* ptr = &c;
		for (int i = 0; i < psize; i++)
		{
			(*this).chains[i] = ptr[i];
		}
	}
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
	{
		(*this).startN = pstartN;
		(*this).stbottomT = pstbottomT;
		(*this).rules = new Rule[(*this).size = psize];
		Rule* ptr = &r;
		for (int i = 0; i < psize; i++)
		{
			rules[i] = ptr[i];
		}
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1, k = 0;
		while (k < (*this).size && rules[k].nn != pnn)
		{
			k++;
		}
		if (k < (*this).size)
		{
			prule = rules[rc = k];
		}

		return rc;
	}
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < (*this).size)
		{
			rc = rules[n];
		}

		return rc;
	}
	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::alphabet_to_char(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		this->chains[nchain].getCChain(buf);
		strcat_s(b, sizeof(buf) + 5, buf);

		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < (*this).size && (*this).chains[j].nt[0] != t)
		{
			j++;
		}
		rc = (j < (*this).size ? j : -1);
		if (rc >= 0)
		{
			pchain = chains[rc];
		}

		return rc;
	}
	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
		{
			b[i] = Chain::alphabet_to_char(this->nt[i]);
		}
		b[(*this).size] = 0;

		return b;
	}
	Greibach getGreibach()
	{
		return greibach;
	}
}