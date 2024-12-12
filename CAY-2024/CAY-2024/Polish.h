#pragma once
#include "stdafx.h"

#define PN_FILLER '|'

namespace PN
{
	void PN(LT::LexTable& lextable, IT::IdTable& idtable);
	void PolishNotation(int pos, LT::LexTable& lextable, IT::IdTable& idtable, char endSymbol);
	int GetPriority(char symbol);
	bool isOperationSign(char signSymbol);
}