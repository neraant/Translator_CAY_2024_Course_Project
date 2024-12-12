#include "stdafx.h"

LT::Entry::Entry()
{
	(*this).lexema = NULL;
	(*this).sn = 0;
	(*this).idxTI = TI_NULLIDX;
}

LT::Entry::Entry(char lexema, int sn, int idxTI)
{
	(*this).lexema = lexema;
	(*this).sn = sn;
	(*this).idxTI = idxTI;
}

LT::LexTable LT::Create(int size)
{
	if (size > LT_MAXSIZE)
	{
		throw ERROR_THROW(120);
	}

	LexTable lextable;
	lextable.maxsize = size;
	lextable.size = 0;
	lextable.table = new Entry[size];

	return lextable;
}

void LT::Add(LexTable& lextable, Entry entry)
{
	if (lextable.size >= LT_MAXSIZE)
	{
		throw ERROR_THROW(121);
	}

	lextable.table[lextable.size++] = entry;
}

LT::Entry LT::GetEntry(LexTable& lextable, int n)
{
	if (n < 0 || n > LT_MAXSIZE)
	{
		throw ERROR_THROW(122);
	}

	return lextable.table[n];
}

void LT::WriteTable(LexTable& lextable)
{
	std::ofstream LTfile(LT_FILENAME);

	if (!LTfile.is_open())
	{
		throw ERROR_THROW(114);
	}

	int currentLine = 1;
	LTfile << currentLine << '\t';
	for (int i = 0; i < lextable.size; i++)
	{
		LT::Entry temp = LT::GetEntry(lextable, i);

		if (currentLine != temp.sn)
		{
			currentLine = temp.sn;
			LTfile << '\n' << currentLine << '\t';
		}

		LTfile << temp.lexema;
	}

	LTfile.close();
}

void LT::Delete(LexTable& lextable)
{
	delete[] lextable.table;
	lextable.table = nullptr;
}

