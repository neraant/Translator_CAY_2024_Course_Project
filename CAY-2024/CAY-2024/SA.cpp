#include "stdafx.h"

void SA::SA(LT::LexTable lextable, IT::IdTable idtable)
{
	int curLine = 1;
	int column = 0;
	bool isMainFound = false;

	for (int i = 0; i < lextable.size - 1; ++i)
	{
		if (lextable.table[i].sn != curLine)
		{
			curLine = lextable.table[i].sn;
			column = 1;
		}
		else
		{
			column++;
		}

		switch (lextable.table[i].lexema)
		{
		case LEX_MAIN:
		{
			isMainFound = true;
			CheckMainRet(lextable, idtable, ++i, curLine, column);
			break;
		}
		case LEX_EQUAL_SIGN:
		{
			CheckAssignment(lextable, idtable, i, curLine, column);
			break;
		}
		case LEX_IF:
		{
			CheckCondition(idtable, lextable, i, LEX_IF, curLine, column);
			isNested(lextable, idtable, ++i, LEX_IF, curLine, column);
			break;
		}
		case LEX_ELSE:
		{
			for (int j = i; j < lextable.size; ++j)
			{
				if (lextable.table[j].lexema == LEX_RIGHTBRACE)
				{
					break;
				}
				else if (lextable.table[j].lexema == LEX_IF)
				{
					throw ERROR_THROW(344);
				}
			}
			break;
		}
		case LEX_WHILE:
		{
			if (!CheckCondition(idtable, lextable, i, LEX_WHILE, curLine, column))
			{
				throw ERROR_THROW_IN(306, curLine, column);
			}
			isNested(lextable, idtable, ++i, LEX_WHILE, curLine, column);
			break;
		}
		case LEX_FUNCTION:
		{
			CheckFunction(lextable, idtable, i, curLine, column);
			break;
		}
		case LEX_ID:
		{
			CheckIdentifier(lextable, idtable, i, curLine, column);
			break;
		}
		case LEX_RETURN:
		{
			CheckRet(lextable, idtable, i, curLine, column);
			break;
		}
		default:
			break;
		}
	}

	if (!isMainFound)
	{	
		throw ERROR_THROW(300);
	}
}

void SA::CheckFunction(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column)
{
	IT::Entry func = idtable.table[lextable.table[pos + 1].idxTI];
	bool isRet = false;
	int braceDepth = 0;

	for (int j = ++pos; j < lextable.size; ++j)
	{
		if (lextable.table[j].sn != curLine) {
			curLine = lextable.table[j].sn;
			Column = 1;
		}
		Column++;

		if (lextable.table[j].lexema == LEX_LEFTBRACE)
		{
			++braceDepth;
		}
		else if (lextable.table[j].lexema == LEX_RIGHTBRACE)
		{
			--braceDepth;

			if (braceDepth == 0)
			{
				break;
			}
		}
		else if (lextable.table[j].lexema == LEX_RETURN && braceDepth == 1)
		{
			if (idtable.table[lextable.table[j + 1].idxTI].iddatatype != func.iddatatype && lextable.table[j + 2].lexema == LEX_SEMICOLON)
			{
				throw ERROR_THROW_IN(310, curLine, Column);
			}
			isRet = true;
		}
		if (lextable.table[j].idxTI != TI_NULLIDX && lextable.table[j].lexema == LEX_ID &&
			strcmp(idtable.table[lextable.table[j].idxTI].id, func.id) == 0 && braceDepth == 1)
		{
			throw ERROR_THROW_IN(343, curLine, Column);
		}
	}

	if (!isRet)
	{
		throw ERROR_THROW_IN(309, curLine, Column);
	}
}

void SA::CheckAssignment(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column)
{
	IT::Entry variable = idtable.table[lextable.table[pos - 1].idxTI];
	int valueIdx = lextable.table[pos + 1].idxTI;
	
	IgnoreParentheses(lextable, pos, valueIdx, curLine, Column);

	int logicalOperatorsCounter = 0;
	int isAriphmeticalOperators = false;

	vector<IT::Entry> rightValues;
	for (int j = ++pos; lextable.table[j].lexema != LEX_SEMICOLON; ++j)
	{
		if (lextable.table[j].sn != curLine) {
			curLine = lextable.table[j].sn;
			Column = 1; 
		}
		Column++;

		if (lextable.table[j].lexema == LEX_LEFTPAREN && lextable.table[j - 1].lexema == LEX_ID)
		{
			while (lextable.table[j].lexema != LEX_RIGHTPAREN)
			{
				j++;
			}
		}
		if (lextable.table[j + 1].idxTI != TI_NULLIDX && lextable.table[j].lexema == LEX_DIVISION && idtable.table[lextable.table[j + 1].idxTI].value.vint == 0)
		{
			throw ERROR_THROW_IN(323, curLine, Column);
		}
		if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL) {
			rightValues.push_back(idtable.table[lextable.table[j].idxTI]);
		}
		if (lextable.table[j].lexema == LEX_EQUAL || lextable.table[j].lexema == LEX_MORE || lextable.table[j].lexema == LEX_LESS ||
			lextable.table[j].lexema == LEX_LESS_EQUAL || lextable.table[j].lexema == LEX_MORE_EQUAL || lextable.table[j].lexema == LEX_NOT_EQUAL)
		{
			logicalOperatorsCounter++;
		}
		if (lextable.table[j].lexema == LEX_PLUS || lextable.table[j].lexema == LEX_MINUS || lextable.table[j].lexema == LEX_MULTIPLY ||
			lextable.table[j].lexema == LEX_DIVISION || lextable.table[j].lexema == LEX_PERCENT)
		{
			isAriphmeticalOperators = true;
		}
	}

	if (rightValues.size() == 1)
	{
		if (variable.iddatatype == IT::BOOL)
		{
			if (rightValues[0].iddatatype != IT::BOOL && rightValues[0].iddatatype != IT::INT)
			{
				throw ERROR_THROW_IN(334, curLine, Column);
			}
		}
		else if (variable.iddatatype != idtable.table[valueIdx].iddatatype)
		{
			throw ERROR_THROW_IN(304, curLine, Column);
		}
	}


	for (int j = 0; j < rightValues.size() - 1; ++j)
	{
		if (rightValues[j].iddatatype == IT::STR && rightValues[j + 1].iddatatype == IT::STR)
		{
			throw ERROR_THROW_IN(335, curLine, Column);
		}
		if (variable.iddatatype == IT::BOOL)
		{
			if (rightValues.size() != 2 || logicalOperatorsCounter != 1)
			{
				throw ERROR_THROW_IN(334, curLine, Column);
			}
		}
		else if (variable.iddatatype == IT::STR)
		{
			if (rightValues.size() > 1)
			{
				throw ERROR_THROW_IN(335, curLine, Column);
			}
		}
		else if (rightValues[j].iddatatype != rightValues[j + 1].iddatatype)
		{
			throw ERROR_THROW_IN(304, curLine, Column);
		}
	}
	pos--;
}

void SA::IgnoreParentheses(LT::LexTable& lextable, int& pos, int& valueIdx, int curLine, int Column)
{
	if (lextable.table[pos + 1].lexema == LEX_LEFTPAREN)
	{
		for (int j = ++pos; lextable.table[j].lexema == LEX_LEFTPAREN; ++j)
		{
			if (lextable.table[j].sn != curLine) {
				curLine = lextable.table[j].sn;
				Column = 1; 
			}
			Column++;
			
			valueIdx = lextable.table[j + 1].idxTI;
		}
	}
}

bool SA::CheckCondition(IT::IdTable& idtable, LT::LexTable& lextable, int pos, char conditionLexema, int curLine, int Column)
{
	vector<LT::Entry> values;
	bool isRightType = false;

	for (int k = pos; k < lextable.size; ++k)
	{
		if (lextable.table[k].sn != curLine) {
			curLine = lextable.table[k].sn;
			Column = 1;
		}
		Column++;

		if (lextable.table[k].lexema == LEX_RIGHTPAREN && lextable.table[k + 1].lexema == LEX_LEFTBRACE)
		{
			break;
		}
		if (lextable.table[k].lexema == LEX_ID || lextable.table[k].lexema == LEX_LITERAL)
		{
			if (lextable.table[k].idxTI != TI_NULLIDX && lextable.table[k].lexema == LEX_ID && idtable.table[lextable.table[k].idxTI].idtype == IT::F)
			{
				throw ERROR_THROW_IN(340, curLine, Column);
			}
			if (lextable.table[k].lexema == LEX_ID && lextable.table[k + 1].lexema == LEX_LEFTPAREN)
			{
				while (lextable.table[k].lexema != LEX_RIGHTPAREN)
				{
					k++;
				}
			}
			values.push_back(lextable.table[k]);
		}
		if (lextable.table[k].lexema == LEX_PLUS || lextable.table[k].lexema == LEX_MINUS ||
			lextable.table[k].lexema == LEX_DIVISION || lextable.table[k].lexema == LEX_MULTIPLY ||
			lextable.table[k].lexema == LEX_PERCENT)
		{
			throw ERROR_THROW_IN(320, curLine, Column);
		}
		else
		{
			continue;
		}
	}

	if (CheckConditionValuesType(values, idtable, IT::IDDATATYPE::INT, conditionLexema, curLine, Column))
	{
		isRightType = true;
	}
	else if (CheckConditionValuesType(values, idtable, IT::IDDATATYPE::STR, conditionLexema, curLine, Column))
	{
		isRightType = true;
	}
	else if (CheckConditionValuesType(values, idtable, IT::IDDATATYPE::BOOL, conditionLexema, curLine, Column))
	{
		isRightType = true;
	}
	else
	{
		isRightType = false;
	}

	return isRightType;
}

bool SA::CheckConditionValuesType(const vector<LT::Entry>& values, IT::IdTable& idtable, IT::IDDATATYPE expectedType, char conditionLexema, int curLine, int Column) {
	if (values.size() > 2) {
		throw ERROR_THROW_IN(307, curLine, Column);
	}
	else if (values.empty())
	{
		throw ERROR_THROW_IN(313, curLine, Column);
	}
	else if (values.size() == 1 && conditionLexema == LEX_WHILE)
	{
		throw ERROR_THROW_IN(319, curLine, Column);
	}
	else if (values.size() == 1 && conditionLexema == LEX_IF)
	{
		throw ERROR_THROW_IN(317, curLine, Column);
	}

	for (const auto& entry : values) {
		if (idtable.table[entry.idxTI].iddatatype != expectedType) {
			return false;
		}
	}

	if (conditionLexema == LEX_IF)
	{
		if (values.size() != 2)
		{
			throw ERROR_THROW_IN(316, curLine, Column);
		}
	}
	if (conditionLexema == LEX_WHILE)
	{
		if (expectedType == IT::STR)
		{
			return false;
		}
		else if (expectedType == IT::BOOL)
		{
			if (values.size() == 1)
			{
				throw ERROR_THROW_IN(318, curLine, Column);
			}
			else
			{
				throw ERROR_THROW_IN(306, curLine, Column);
			}
		}
		else if (expectedType == IT::INT)
		{
			int literalCounter = 0;
			for (const auto& entry : values) {
				if (entry.lexema == LEX_LITERAL) {
					literalCounter++;
				}
			}
			if (literalCounter == values.size())
			{
				throw ERROR_THROW_IN(318, curLine, Column);
			}
		}
		if (values.size() == 1)
		{
			throw ERROR_THROW_IN(319, curLine, Column);
		}
	}

	return true;
}

void SA::CheckIdentifier(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column)
{
	if (strcmp(idtable.table[lextable.table[pos].idxTI].id, "date") == 0 ||
		strcmp(idtable.table[lextable.table[pos].idxTI].id, "rand") == 0 ||
		strcmp(idtable.table[lextable.table[pos].idxTI].id, "isEmpty") == 0 ||
		strcmp(idtable.table[lextable.table[pos].idxTI].id, "isEven") == 0 ||
		strcmp(idtable.table[lextable.table[pos].idxTI].id, "len") == 0)
	{
		int paramsCounter = 0;
		for (int j = pos + 2; j < lextable.size; ++j)
		{
			if (lextable.table[j].sn != curLine) {
				curLine = lextable.table[j].sn;
				Column = 1;
			}
			else
			{
				Column++;
			}

			if ((lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL) &&
				lextable.table[j].idxTI != TI_NULLIDX && idtable.table[lextable.table[j].idxTI].idtype != IT::F)
			{
				paramsCounter++;
			}
			if (lextable.table[j].lexema == LEX_RIGHTPAREN)
			{
				break;
			}
		}

		if (strcmp(idtable.table[lextable.table[pos].idxTI].id, "isEmpty") == 0)
		{
			if (paramsCounter != 1)
			{
				throw ERROR_THROW_IN(336, curLine, Column);

			}
			if (idtable.table[lextable.table[pos + 2].idxTI].iddatatype != IT::STR)
			{
				throw ERROR_THROW_IN(327, curLine, Column);
			}
		}
		else if (strcmp(idtable.table[lextable.table[pos].idxTI].id, "isEven") == 0)
		{
			if (paramsCounter != 1)
			{
				throw ERROR_THROW_IN(337, curLine, Column);

			}
			if (idtable.table[lextable.table[pos + 2].idxTI].iddatatype != IT::INT)
			{
				throw ERROR_THROW_IN(328, curLine, Column);
			}
		}
		else if (strcmp(idtable.table[lextable.table[pos].idxTI].id, "len") == 0)
		{
			if (paramsCounter != 1)
			{
				throw ERROR_THROW_IN(338, curLine, Column);

			}
			if (idtable.table[lextable.table[pos + 2].idxTI].iddatatype != IT::STR)
			{
				throw ERROR_THROW_IN(329, curLine, Column);
			}
		}
		else
		{
			if (paramsCounter > 0)
			{
				if (strcmp(idtable.table[lextable.table[pos].idxTI].id, "date") == 0) {
					throw ERROR_THROW_IN(325, curLine, Column);
				}
				else if (strcmp(idtable.table[lextable.table[pos].idxTI].id, "rand") == 0) {
					throw ERROR_THROW_IN(324, curLine, Column);
				}
			}
		}
		paramsCounter = 0;
		return;
	}
	else if (lextable.table[pos].idxTI != TI_NULLIDX && idtable.table[lextable.table[pos].idxTI].idtype == IT::F && lextable.table[pos - 1].lexema != LEX_FUNCTION)
	{
		IT::Entry func = idtable.table[lextable.table[pos].idxTI];
		int funcFirstLexemeIdx = func.idxfirstLE;
		vector<IT::IDDATATYPE> paramType;

		if (lextable.table[pos + 1].lexema == LEX_LEFTPAREN && (lextable.table[pos - 1].lexema == LEX_SEMICOLON ||
			lextable.table[pos - 1].lexema == LEX_LEFTBRACE || lextable.table[pos - 1].lexema == LEX_RIGHTBRACE))
		{
			throw ERROR_THROW_IN(345, curLine, Column);
		}

		for (int j = ++pos; lextable.table[j].lexema != LEX_RIGHTPAREN; ++j)
		{
			if (lextable.table[j].sn != curLine) {
				curLine = lextable.table[j].sn;
				Column = 1;
			}
			else
			{
				Column++;
			}

			if (lextable.table[j].lexema == LEX_ID || lextable.table[j].lexema == LEX_LITERAL)
			{
				paramType.push_back(idtable.table[lextable.table[j].idxTI].iddatatype);
			}
		}

		vector<IT::IDDATATYPE> expectedParamTypes;
		for (size_t j = 0; j < lextable.size; ++j)
		{
			if (lextable.table[j].sn != curLine) {
				curLine = lextable.table[j].sn;
				Column = 1;
			}
			else
			{
				Column++;
			}

			if (lextable.table[j].idxTI != TI_NULLIDX && strcmp(idtable.table[lextable.table[j].idxTI].id, func.id) == 0)
			{
				for (size_t k = ++j; lextable.table[k].lexema != LEX_RIGHTPAREN; ++k)
				{
					if (lextable.table[k].sn != curLine) {
						curLine = lextable.table[k].sn;
						Column = 1;
					}
					else
					{
						Column++;
					}

					if (lextable.table[k].lexema == LEX_ID)
					{
						expectedParamTypes.push_back(idtable.table[lextable.table[k].idxTI].iddatatype);
					}
				}
				break;
			}
		}

		if (expectedParamTypes.size() != paramType.size())
		{
			throw ERROR_THROW_IN(314, curLine, Column);
		}
		for (size_t k = 0; k < expectedParamTypes.size(); ++k) {
			if (expectedParamTypes[k] != paramType[k]) {
				throw ERROR_THROW_IN(315, curLine, Column);
			}
		}
	}
}

void SA::CheckMainRet(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column)
{
	for (int j = pos; j < lextable.size; ++j)
	{
		if (lextable.table[j].sn != curLine) {
			curLine = lextable.table[j].sn;
			Column = 1;
		}
		Column++;
		
		if (lextable.table[j].lexema == LEX_RETURN)
		{
			throw ERROR_THROW_IN(322, curLine, Column);
		}
	}
}

void SA::CheckRet(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column)
{
	for (int j = pos; j < lextable.size; ++j)
	{
		if (lextable.table[j].sn != curLine) {
			curLine = lextable.table[j].sn;
			Column = 1;
		}
		Column++;

		if (lextable.table[j].lexema == LEX_SEMICOLON)
		{
			break;
		}
		else if (lextable.table[j].lexema == LEX_PLUS || lextable.table[j].lexema == LEX_MINUS || lextable.table[j].lexema == LEX_MULTIPLY ||
				lextable.table[j].lexema == LEX_DIVISION || lextable.table[j].lexema == LEX_PERCENT)
		{
			throw ERROR_THROW_IN(332, curLine, Column);
		}
		else if (lextable.table[j].lexema == LEX_EQUAL || lextable.table[j].lexema == LEX_NOT_EQUAL || lextable.table[j].lexema == LEX_MORE ||
			lextable.table[j].lexema == LEX_LESS || lextable.table[j].lexema == LEX_LESS_EQUAL || lextable.table[j].lexema == LEX_MORE_EQUAL)
		{
			throw ERROR_THROW_IN(341, curLine, Column);
		}
		else if (lextable.table[j].idxTI != TI_NULLIDX && lextable.table[j].lexema == LEX_ID && lextable.table[j + 1].lexema == LEX_LEFTPAREN)
		{
			throw ERROR_THROW_IN(342, curLine, Column);
		}
	}
}

void SA::isNested(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, char assignSymbol, int curLine, int Column)
{
	int braceDepth = 0;

	for (int j = ++pos; j < lextable.size; ++j)
	{
		if (lextable.table[j].sn != curLine) {
			curLine = lextable.table[j].sn;
			Column = 1;
		}
		Column++;

		if (lextable.table[j].lexema == LEX_LEFTBRACE)
		{
			++braceDepth;
		}
		else if (lextable.table[j].lexema == LEX_RIGHTBRACE)
		{
			--braceDepth;

			if (braceDepth == 0)
			{
				break;
			}
		}
		else if (lextable.table[j].lexema == assignSymbol && braceDepth == 1)
		{
			throw ERROR_THROW_IN(344, curLine, Column);
		}
	}
}