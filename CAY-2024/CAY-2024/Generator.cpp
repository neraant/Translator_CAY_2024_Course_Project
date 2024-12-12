#include "stdafx.h"

void GEN::AsmGeneration(LA::LEX lex, Out::OUT& fout)
{
	GEN::GenerateHead(lex, fout);

	GEN::GenerateConst(lex, fout);

	GEN::GenerateData(lex, fout);

	GEN::GenerateCode(lex, fout);

	GEN::GenerateFooter(lex, fout);
}

void GEN::GenerateHead(LA::LEX lex, Out::OUT& fout)
{
	// HEAD
	(*fout.stream) << HEAD;

	// PROTO
	(*fout.stream) << PROTOTYPES;
}

void GEN::GenerateConst(LA::LEX lex, Out::OUT& fout)
{
	// .CONST
	(*fout.stream) << ".CONST";
	for (size_t i = 0; i < lex.lextable.size; ++i)
	{
		LT::Entry lexemaLt = lex.lextable.table[i];

		if (lexemaLt.lexema == LEX_LITERAL)
		{
			IT::Entry lexemaId = lex.idtable.table[lexemaLt.idxTI];

			switch (lexemaId.iddatatype)
			{
			case IT::STR:
			{
				if (lexemaId.value.vstr.len == 0)
				{
					(*fout.stream) << "\n" << lexemaId.id << " BYTE " << "\"" << lexemaId.value.vstr.str << " \"" << ", 0";
				}
				else
				{
					(*fout.stream) << "\n" << lexemaId.id << " BYTE " << "\"" << lexemaId.value.vstr.str << "\"" << ", 0";
				}
				break;
			}
			case IT::INT:
			{
				(*fout.stream) << "\n" << lexemaId.id << " DWORD " << lexemaId.value.vint;
				break;
			}
			case IT::BOOL:
			{
				(*fout.stream) << "\n" << lexemaId.id << " DWORD " << lexemaId.value.vbool;
				break;
			}
			default:
				break;
			}
		}
	}
}

void GEN::GenerateData(LA::LEX lex, Out::OUT& fout)
{
	// .DATA
	(*fout.stream) << "\n\n.DATA";
	for (size_t i = 0; i < lex.lextable.size; i++)
	{
		if (lex.idtable.table[i].idtype == IT::V)
		{
			switch (lex.idtable.table[i].iddatatype)
			{
			case IT::INT:
			{
				(*fout.stream) << "\n" << (lex.idtable.table[i].id + (string)"_" + (string)(lex.idtable.table[i].scope)) << " DWORD " << lex.idtable.table[i].value.vint;
				break;
			}
			case IT::STR:
			{
				(*fout.stream) << "\n" << (lex.idtable.table[i].id + (string)"_" + (string)(lex.idtable.table[i].scope)) << " BYTE 14 DUP(0)";
				break;
			}
			case IT::BOOL:
			{
				(*fout.stream) << "\n" << (lex.idtable.table[i].id + (string)"_" + (string)(lex.idtable.table[i].scope)) << " DWORD " << lex.idtable.table[i].value.vbool;
				break;
			}
			default:
				break;
			}
		}
	}
}

void GEN::GenerateCode(LA::LEX lex, Out::OUT& fout)
{
	int column = 1;
	int curLine = 0;

	stack<string> scopeStack;

	IT::Entry funcIT;
	bool isFunc = false;
	bool isReturned = false;

	bool isIf = false;
	bool isElse = false;
	string ifScopeName;
	int ifScopeCounter = 0;

	bool isWhile = false;
	int whileScopeCounter = 0;
	string whileScopeName;
	string counterName;

	bool isArOperations = false;
	bool isLogicalOperations = false;

	vector<IT::Entry> paramsContainer;

	// .CODE
	(*fout.stream) << "\n\n.CODE";
	for (size_t i = 0; i < lex.lextable.size; i++)
	{
		if (lex.lextable.table[i].sn != curLine) {
			curLine = lex.lextable.table[i].sn;
			column = 1;
		}
		column++;

		switch (lex.lextable.table[i].lexema)
		{
		case LEX_FUNCTION:
		{
			i = ProcessFunction(lex, fout, i, curLine, column, funcIT, isFunc, paramsContainer);
			break;
		}
		case LEX_IF:
		{
			i = ProcessIf(lex, fout, i, curLine, column, isIf, isElse, ifScopeName, scopeStack, ifScopeCounter);
			break;
		}
		case LEX_ELSE:
			(*fout.stream) << "\n\n; Блок else";
			(*fout.stream) << "\nelse_" << ifScopeName << ":";
			break;
		case LEX_WHILE:
			i = ProcessWhile(lex, fout, i, curLine, column, isWhile, whileScopeName, scopeStack, whileScopeCounter, counterName);
			break;
		case LEX_RETURN:
		{
			i = ProcessReturn(lex, fout, i, isReturned);
			break;
		}
		case LEX_MAIN:
		{
			(*fout.stream) << "\n\nmain PROC\nSTART :";
			break;
		}
		case LEX_WRITE:
		{
			i = ProcessWrite(lex, fout, isArOperations, isLogicalOperations, i, curLine, column, "Write");
			break;
		}
		case LEX_WRITELINE:
		{
			i = ProcessWrite(lex, fout, isArOperations, isLogicalOperations, i, curLine, column, "WriteLine");
			break;
		}
		case LEX_EQUAL_SIGN:
		{
			i = ProcessEqual(lex, fout, isArOperations, isLogicalOperations, i, curLine, column);
			break;
		}
		case LEX_RIGHTBRACE:
		{
			i = ProcessRightbrace(lex, fout, i, isFunc, isReturned, isElse, isIf, isWhile, funcIT, scopeStack, ifScopeName, whileScopeName, counterName);
			break;
		}
		default:
			break;
		}
	}
}

void GEN::GenerateFooter(LA::LEX lex, Out::OUT& fout)
{
	// FOOT
	(*fout.stream) << FOOTER;
}

int GEN::ProcessFunction(LA::LEX lex, Out::OUT& fout, int i, int curLine, int column, IT::Entry& funcIT, bool& isFunc, vector<IT::Entry>& paramsContainer)
{
	funcIT = lex.idtable.table[lex.lextable.table[i + 1].idxTI];
	isFunc = true;

	(*fout.stream) << "\n\n" << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i + 1].idxTI].scope << " PROC";

	if (lex.lextable.table[i + 3].lexema != LEX_LEFTPAREN)
	{
		for (size_t j = i; lex.lextable.table[j].lexema != LEX_LEFTBRACE; ++j)
		{
			if (lex.lextable.table[j].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[j].idxTI].idtype == IT::P)
			{
				paramsContainer.push_back(lex.idtable.table[lex.lextable.table[j].idxTI]);
			}
		}

		for (int j = paramsContainer.size() - 1; j < paramsContainer.size(); j--)
		{
			(*fout.stream) << ", " << paramsContainer[j].id + (string)"_" + paramsContainer[j].scope;
			switch (paramsContainer[j].iddatatype)
			{
			case IT::INT:
				(*fout.stream) << " : DWORD";
				break;
			case IT::STR:
				(*fout.stream) << " : DWORD";
				break;
			case IT::BOOL:
				(*fout.stream) << " : DWORD";
				break;
			default:
				break;
			}

			if (j == 0) break;
		}
		paramsContainer.clear();
	}
	return i;
}

int GEN::ProcessIf(LA::LEX lex, Out::OUT& fout, int i, int curLine, int column, bool& isIf, bool& isElse, string& ifScopeName, stack<string>& scopeStack, int& ifScopeCounter)
{
	isIf = true;
	ifScopeName = lex.idtable.table[lex.lextable.table[i + 2].idxTI].scope + to_string(ifScopeCounter);
	scopeStack.push(ifScopeName);
	ifScopeCounter++;
	int bracesDepth = 0;
	int initialDepth = bracesDepth;

	for (int j = i; j < lex.lextable.size; ++j)
	{
		if (lex.lextable.table[j].lexema == LEX_LEFTBRACE)
		{
			bracesDepth++;
		}
		if (lex.lextable.table[j].lexema == LEX_RIGHTBRACE)
		{
			bracesDepth--;
			if (lex.lextable.table[j + 1].lexema == LEX_ELSE && bracesDepth == initialDepth)
			{
				isElse = true;
				break;
			}
		}
	}

	(*fout.stream) << "\n\t; Начало блока условия";
	for (int j = ++i; lex.lextable.table[j].lexema != LEX_LEFTBRACE; j++)
	{
		if (lex.lextable.table[j].lexema == LEX_LEFTPAREN)
			continue;
		if (lex.lextable.table[j].lexema == LEX_LITERAL && lex.lextable.table[j + 1].lexema == LEX_LITERAL)
		{
			if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != IT::STR)
			{
				(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id;

				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].value.vstr.len;
				}
				else
				{
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;
				}
				break;
			}
			else
			{
				(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].value.vstr.len;

				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].value.vstr.len;
				}
				else
				{
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;
				}
				break;
			}
		}
		else if (lex.lextable.table[j].lexema == LEX_LITERAL && lex.lextable.table[j + 1].lexema == LEX_ID)
		{
			if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != IT::STR)
			{
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
					(*fout.stream) << "\n\tcall lenCAY";
					(*fout.stream) << "\n\tmov ebx, eax";
					(*fout.stream) << "\n\txor eax, eax";
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
					(*fout.stream) << "\n\tcmp eax, ebx";
				}
				else
				{
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
				}
				break;
			}
			else
			{
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
				(*fout.stream) << "\n\tcall lenCAY";
				(*fout.stream) << "\n\tmov ebx, eax";
				(*fout.stream) << "\n\txor eax, eax";
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
					(*fout.stream) << "\n\tcall lenCAY";
					(*fout.stream) << "\n\tcmp ebx, eax";
				}
				else
				{
					(*fout.stream) << "\n\tcmp ebx, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
				}

				break;
			}
		}
		else if (lex.lextable.table[j].lexema == LEX_ID && lex.lextable.table[j + 1].lexema == LEX_LITERAL)
		{
			if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != IT::STR)
			{
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;
					(*fout.stream) << "\n\tcall lenCAY";
					(*fout.stream) << "\n\tmov ebx, eax";
					(*fout.stream) << "\n\txor eax, eax";
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
					(*fout.stream) << "\n\tcmp eax, ebx";
				}
				else
				{
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;
				}
				break;
			}
			else
			{
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				(*fout.stream) << "\n\tcall lenCAY";
				(*fout.stream) << "\n\tmov ebx, eax";
				(*fout.stream) << "\n\txor eax, eax";
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;
					(*fout.stream) << "\n\tcall lenCAY";
					(*fout.stream) << "\n\tcmp ebx, eax";
				}
				else
				{
					(*fout.stream) << "\n\tcmp ebx, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;
				}

				break;
			}
		}
		else if (lex.lextable.table[j].lexema == LEX_ID && lex.lextable.table[j + 1].lexema == LEX_ID)
		{
			if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype != IT::STR)
			{
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
					(*fout.stream) << "\n\tcall lenCAY";
					(*fout.stream) << "\n\tmov ebx, eax";
					(*fout.stream) << "\n\txor eax, eax";
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
					(*fout.stream) << "\n\tcmp eax, ebx";
				}
				else
				{
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
					(*fout.stream) << "\n\tcmp eax, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				}
				break;
			}
			else
			{
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				(*fout.stream) << "\n\tcall lenCAY";
				(*fout.stream) << "\n\tmov ebx, eax";
				(*fout.stream) << "\n\txor eax, eax";
				if (lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
					(*fout.stream) << "\n\tcall lenCAY";
					(*fout.stream) << "\n\tcmp ebx, eax";
				}
				else
				{
					(*fout.stream) << "\n\tcmp ebx, " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				}

				break;
			}
		}
		++i;
	}

	switch (lex.lextable.table[i + 3].lexema)
	{
	case LEX_EQUAL:
		(*fout.stream) << "\n\tje " << ifScopeName;
		if (isElse)
		{
			(*fout.stream) << "\n\tjmp else_" << ifScopeName;
		}
		else
		{
			(*fout.stream) << "\n\tjmp end_" << ifScopeName;
		}
		break;
	case LEX_NOT_EQUAL:
		(*fout.stream) << "\n\tjne " << ifScopeName;
		if (isElse)
		{
			(*fout.stream) << "\n\tjmp else_" << ifScopeName;
		}
		else
		{
			(*fout.stream) << "\n\tjmp end_" << ifScopeName;
		}
		break;
	case LEX_MORE:
		(*fout.stream) << "\n\tjg " << ifScopeName;
		if (isElse)
		{
			(*fout.stream) << "\n\tjmp else_" << ifScopeName;
		}
		else
		{
			(*fout.stream) << "\n\tjmp end_" << ifScopeName;
		}
		break;
	case LEX_LESS:
		(*fout.stream) << "\n\tjl " << ifScopeName;
		if (isElse)
		{
			(*fout.stream) << "\n\tjmp else_" << ifScopeName;
		}
		else
		{
			(*fout.stream) << "\n\tjmp end_" << ifScopeName;
		}
		break;
	case LEX_MORE_EQUAL:
		(*fout.stream) << "\n\tjge " << ifScopeName;
		if (isElse)
		{
			(*fout.stream) << "\n\tjmp else_" << ifScopeName;
		}
		else
		{
			(*fout.stream) << "\n\tjmp end_" << ifScopeName;
		}
		break;
	case LEX_LESS_EQUAL:
		(*fout.stream) << "\n\tjle " << ifScopeName;
		if (isElse)
		{
			(*fout.stream) << "\n\tjmp else_" << ifScopeName;
		}
		else
		{
			(*fout.stream) << "\n\tjmp end_" << ifScopeName;
		}
		break;
	default:
		break;
	}

	(*fout.stream) << "\n\n; Блок if";
	(*fout.stream) << "\n" << ifScopeName << ":";
	return i;
}

int GEN::ProcessWhile(LA::LEX lex, Out::OUT& fout, int i, int curLine, int column, bool& isWhile, string& whileScopeName, stack<string>& scopeStack, int& whileScopeCounter, string& counterName)
{
	isWhile = true;
	whileScopeName = "while" + to_string(whileScopeCounter);
	scopeStack.push(whileScopeName);
	whileScopeCounter++;

	(*fout.stream) << "\n\t; Начало цикла while";
	for (int j = ++i; lex.lextable.table[j].lexema != LEX_LEFTBRACE; j++)
	{
		if (lex.lextable.table[j].lexema == LEX_LEFTPAREN)
			continue;

		(*fout.stream) << "\n\tmov ecx, ";
		if (lex.lextable.table[j].lexema == LEX_ID) {
			(*fout.stream) << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
			counterName = lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
		}
		else if (lex.lextable.table[j].lexema == LEX_LITERAL) {
			(*fout.stream) << lex.idtable.table[lex.lextable.table[j].idxTI].id;
			counterName = lex.idtable.table[lex.lextable.table[j].idxTI].id;
		}

		(*fout.stream) << "\nstart_" << whileScopeName << ":";

		(*fout.stream) << "\n\tcmp ecx, ";
		if (lex.lextable.table[j + 1].lexema == LEX_ID)
			(*fout.stream) << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
		else if (lex.lextable.table[j + 1].lexema == LEX_LITERAL)
			(*fout.stream) << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id;

		switch (lex.lextable.table[i + 3].lexema)
		{
		case LEX_EQUAL:
			(*fout.stream) << "\n\tjne end_" << whileScopeName;
			break;
		case LEX_NOT_EQUAL:
			(*fout.stream) << "\n\tje end_" << whileScopeName;
			break;
		case LEX_MORE:
			(*fout.stream) << "\n\tjle end_" << whileScopeName;
			break;
		case LEX_LESS:
			(*fout.stream) << "\n\tjge end_" << whileScopeName;
			break;
		case LEX_MORE_EQUAL:
			(*fout.stream) << "\n\tjl end_" << whileScopeName;
			break;
		case LEX_LESS_EQUAL:
			(*fout.stream) << "\n\tjg end_" << whileScopeName;
			break;
		default:
			break;
		}
		++i;
		break;
	}
	return i;
}

int GEN::ProcessEqual(LA::LEX lex, Out::OUT& fout, bool& isArOperations, bool& isLogicalOperations, int i, int curLine, int column)
{
	int equalPos = i;
	for (int j = i; lex.lextable.table[j].lexema != LEX_SEMICOLON && lex.lextable.table[j].lexema != PN_FILLER; ++j)
	{
		if (lex.lextable.table[j].lexema == LEX_PLUS || lex.lextable.table[j].lexema == LEX_MINUS || lex.lextable.table[j].lexema == LEX_MULTIPLY || lex.lextable.table[j].lexema == LEX_DIVISION || lex.lextable.table[j].lexema == LEX_PERCENT)
		{
			isArOperations = true;
			break;
		}
		if (lex.lextable.table[j].lexema == LEX_MORE || lex.lextable.table[j].lexema == LEX_LESS || lex.lextable.table[j].lexema == LEX_MORE_EQUAL ||
			lex.lextable.table[j].lexema == LEX_LESS_EQUAL || lex.lextable.table[j].lexema == LEX_EQUAL || lex.lextable.table[j].lexema == LEX_NOT_EQUAL)
		{
			isLogicalOperations = true;
			break;
		}
	}
	if (isArOperations)
	{
		isArOperations = false;
		i = GEN::ProcessArithmeticOperations(lex, fout, i, true, curLine, column);
		return i;
	}
	if (isLogicalOperations)
	{
		isLogicalOperations = false;
		i = GEN::ProcessLogicalOperations(lex, fout, i, true, curLine, column);
		return i;
	}

	if (lex.lextable.table[i + 2].lexema == LEX_SEMICOLON)
	{
		if (lex.lextable.table[i + 1].lexema == LEX_LITERAL)
		{
			if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype == IT::STR)
			{
				(*fout.stream) << "\n\t; Копируем строку";
				(*fout.stream) << "\n\tmov eax, offset " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id;
				(*fout.stream) << "\n\tpush eax";
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[i - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i - 1].idxTI].scope;
				(*fout.stream) << "\n\tcall copy_string";
				(*fout.stream) << "\n";
			}
			else
			{
				(*fout.stream) << "\n\t; Копируем значение";
				(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id;
				(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[i - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i - 1].idxTI].scope << ", eax";
				(*fout.stream) << "\n";
			}
		}
		else if (lex.lextable.table[i + 1].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype != IT::F)
		{
			if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype == IT::STR)
			{
				(*fout.stream) << "\n\t; Копируем строку";
				(*fout.stream) << "\n\tmov eax, offset " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i + 1].idxTI].scope;
				(*fout.stream) << "\n\tpush eax";
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[i - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i - 1].idxTI].scope;
				(*fout.stream) << "\n\tcall copy_string";
				(*fout.stream) << "\n";
			}
			else
			{
				(*fout.stream) << "\n\t; Копируем значение";
				(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i + 1].idxTI].scope;
				(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[i - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i - 1].idxTI].scope << ", eax";
				(*fout.stream) << "\n";
			}
		}
	}
	else if (lex.lextable.table[i + 1].lexema == '~')
	{
		i = GEN::GetFuncParams(lex, fout, i);

		if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "date") == 0)
		{
			(*fout.stream) << "\n\t; Вызов функции";
			(*fout.stream) << "\n\tcall dateCAY";
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope;
			(*fout.stream) << "\n\tcall copy_string";
			(*fout.stream) << "\n";
			return i;
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "rand") == 0)
		{
			(*fout.stream) << "\n\t; Вызов функции";
			(*fout.stream) << "\n\tcall randCAY";
			(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope << ", eax";
			(*fout.stream) << "\n";
			return i;
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "isEmpty") == 0)
		{
			(*fout.stream) << "\n\t; Вызов функции";
			(*fout.stream) << "\n\tcall isEmptyCAY";
			(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope << ", eax";
			(*fout.stream) << "\n";
			return i;
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "isEven") == 0)
		{
			(*fout.stream) << "\n\t; Вызов функции";
			(*fout.stream) << "\n\tcall isEvenCAY";
			(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope << ", eax";
			(*fout.stream) << "\n";
			return i;
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "len") == 0)
		{
			(*fout.stream) << "\n\t; Вызов функции";
			(*fout.stream) << "\n\tcall lenCAY";
			(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope << ", eax";
			(*fout.stream) << "\n";
			return i;
		}

		(*fout.stream) << "\n\t; Вызов функции";
		(*fout.stream) << "\n\tcall " << lex.idtable.table[lex.lextable.table[i].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i].idxTI].scope;
		if (lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype == IT::STR)
		{
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope;
			(*fout.stream) << "\n\tcall copy_string";
		}
		else
		{
			(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[equalPos - 1].idxTI].scope << ", eax";
		}
		(*fout.stream) << "\n\txor eax, eax";
		(*fout.stream) << "\n";
	}
	return i;
}

int GEN::ProcessReturn(LA::LEX lex, Out::OUT& fout, int i, bool& isReturned)
{
	isReturned = true;
	if (lex.lextable.table[i + 1].lexema == LEX_LITERAL)
	{
		switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
		{
		case IT::INT:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].value.vint;
			break;
		}
		case IT::STR:
		{
			(*fout.stream) << "\n\tmov eax, offset " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id;
			break;
		}
		case IT::BOOL:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].value.vbool;
			break;
		}
		default:
			break;
		}
	}
	else if (lex.lextable.table[i + 1].lexema == LEX_ID)
	{
		switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
		{
		case IT::INT:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i + 1].idxTI].scope;
			break;
		}
		case IT::STR:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i + 1].idxTI].scope;
			break;
		}
		case IT::BOOL:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i + 1].idxTI].scope;
			break;
		}
		default:
			break;
		}
	}
	(*fout.stream) << "\n\tret";
	return i;
}

int GEN::ProcessRightbrace(LA::LEX lex, Out::OUT& fout, int i, bool& isFunc, bool& isReturned, bool& isElse, bool& isIf, bool& isWhile, IT::Entry& funcIT, stack<string>& scopeStack, string& ifScopeName, string& whileScopeName, string& counterName)
{
	if (isFunc && isReturned && !isElse && !isIf && !isWhile)
	{
		(*fout.stream) << "\n" << funcIT.id + (string)"_" + funcIT.scope << " ENDP";
		isFunc = false;
	}
	if (isIf && !scopeStack.empty() && scopeStack.top() == ifScopeName)
	{
		(*fout.stream) << "\n\tjmp" << " end_" << ifScopeName;

		if (!isElse)
		{
			(*fout.stream) << "\n\n; Конец блока if / else";
			(*fout.stream) << "\nend_" << ifScopeName << ":";
			scopeStack.pop();
		}

		isIf = false;
	}
	else if (isElse && !scopeStack.empty() && scopeStack.top() == ifScopeName)
	{
		(*fout.stream) << "\n\n; Конец блока if / else";
		(*fout.stream) << "\nend_" << ifScopeName << ":";
		scopeStack.pop();
		isElse = false;
	}
	if (isWhile && !scopeStack.empty() && scopeStack.top() == whileScopeName)
	{
		(*fout.stream) << "\n\tmov ecx, " << counterName;
		(*fout.stream) << "\n\tjmp start_" << whileScopeName;
		(*fout.stream) << "\n\n; Конец цикла while";
		(*fout.stream) << "\nend_" << whileScopeName << ":";
		scopeStack.pop();
		isWhile = false;
	}
	return i;
}

int GEN::ProcessWrite(LA::LEX lex, Out::OUT& fout, bool& isArOperations, bool& isLogicalOperations, int i, int curLine, int column, string codeLexema)
{
	(*fout.stream) << "\n\t; Вывод значения";
	if (lex.lextable.table[i + 2].lexema == LEX_RIGHTPAREN)
	{
		throw ERROR_THROW_IN(330, curLine, column);
	}

	for (int j = i + 2; lex.lextable.table[j].lexema != LEX_SEMICOLON && lex.lextable.table[j].lexema != PN_FILLER; ++j)
	{
		if (lex.lextable.table[j].lexema == LEX_PLUS || lex.lextable.table[j].lexema == LEX_MINUS || lex.lextable.table[j].lexema == LEX_MULTIPLY || lex.lextable.table[j].lexema == LEX_DIVISION || lex.lextable.table[j].lexema == LEX_PERCENT)
		{
			isArOperations = true;
			break;
		}
		if (lex.lextable.table[j].lexema == LEX_MORE || lex.lextable.table[j].lexema == LEX_LESS || lex.lextable.table[j].lexema == LEX_MORE_EQUAL ||
			lex.lextable.table[j].lexema == LEX_LESS_EQUAL || lex.lextable.table[j].lexema == LEX_EQUAL || lex.lextable.table[j].lexema == LEX_NOT_EQUAL)
		{
			isLogicalOperations = true;
			break;
		}
	}
	if (isArOperations)
	{
		isArOperations = false;
		i = GEN::ProcessArithmeticOperations(lex, fout, i, false, curLine, column);

		switch (lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype)
		{
		case IT::INT:
			(*fout.stream) << "\n\tcall " << codeLexema << "Int";
			break;
		case IT::STR:
			(*fout.stream) << "\n\tcall " << codeLexema << "Str";
			break;
		case IT::BOOL:
			(*fout.stream) << "\n\tcall " << codeLexema << "Bool";
			break;
		default:
			break;
		}
		return i;
	}
	if (isLogicalOperations)
	{
		isLogicalOperations = false;
		i = GEN::ProcessLogicalOperations(lex, fout, i, false, curLine, column);

		switch (lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype)
		{
		case IT::INT:
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tcall " << codeLexema << "Int";
			break;
		case IT::STR:
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tcall " << codeLexema << "Str";
			break;
		case IT::BOOL:
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tcall " << codeLexema << "Bool";
			break;
		default:
			break;
		}
		return i;
	}

	if (lex.lextable.table[i + 2].lexema == LEX_LITERAL)
	{
		switch (lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype)
		{
		case IT::INT:
		{
			(*fout.stream) << "\n\tpush " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id;
			(*fout.stream) << "\n\tcall " << codeLexema << "Int";
			break;
		}
		case IT::STR:
		{
			(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id;
			(*fout.stream) << "\n\tcall " << codeLexema << "Str";
			break;
		}
		case IT::BOOL:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id;
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tcall " << codeLexema << "Bool";
			break;
		}
		default:
			break;
		}
	}
	else if (lex.lextable.table[i + 2].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype != IT::F)
	{
		switch (lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype)
		{
		case IT::INT:
		{
			(*fout.stream) << "\n\tpush " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << (string)"_" + lex.idtable.table[lex.lextable.table[i + 2].idxTI].scope;
			(*fout.stream) << "\n\tcall " << codeLexema << "Int";
			break;
		}
		case IT::STR:
		{
			(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << (string)"_" + lex.idtable.table[lex.lextable.table[i + 2].idxTI].scope;
			(*fout.stream) << "\n\tcall " << codeLexema << "Str";
			break;
		}
		case IT::BOOL:
		{
			(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << (string)"_" + lex.idtable.table[lex.lextable.table[i + 2].idxTI].scope;
			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n\tcall " << codeLexema << "Bool";
			break;
		}
		default:
			break;
		}
	}
	else
	{
		if (lex.lextable.table[i + 2].lexema == '~')
		{
			i = GEN::GetFuncParams(lex, fout, i);
		}
		else if (lex.lextable.table[i + 2].idxTI != TI_NULLIDX && lex.lextable.table[i + 2].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype == IT::F)
		{
			i = i + 2;
		}

		if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "date") == 0)
		{
			(*fout.stream) << "\n\tcall dateCAY";
			(*fout.stream) << "\n\tpush eax";
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "rand") == 0)
		{
			(*fout.stream) << "\n\tcall randCAY";
			(*fout.stream) << "\n\tpush eax";
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "isEmpty") == 0)
		{
			(*fout.stream) << "\n\tcall isEmptyCAY";
			(*fout.stream) << "\n\tpush eax";
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "isEven") == 0)
		{
			(*fout.stream) << "\n\tcall isEvenCAY";
			(*fout.stream) << "\n\tpush eax";
		}
		else if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "len") == 0)
		{
			(*fout.stream) << "\n\tcall lenCAY";
			(*fout.stream) << "\n\tpush eax";
		}
		else
		{
			(*fout.stream) << "\n\tcall " << lex.idtable.table[lex.lextable.table[i].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[i].idxTI].scope;
			(*fout.stream) << "\n\tpush eax";
		}


		switch (lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype)
		{
		case IT::INT:
			(*fout.stream) << "\n\tcall " << codeLexema << "Int";
			break;
		case IT::STR:
			(*fout.stream) << "\n\tcall " << codeLexema << "Str";
			break;
		case IT::BOOL:
			(*fout.stream) << "\n\tcall " << codeLexema << "Bool";
			break;
		default:
			break;
		}

		(*fout.stream) << "\n\txor eax, eax";
	}
	return i;
}

int GEN::GetFuncParams(LA::LEX lex, Out::OUT& fout, int pos)
{
	for (int j = pos; ; ++j)
	{
		if (lex.lextable.table[j].idxTI != TI_NULLIDX && (lex.idtable.table[lex.lextable.table[j].idxTI].idtype == IT::V || lex.idtable.table[lex.lextable.table[j].idxTI].idtype == IT::L))
		{
			if (lex.lextable.table[j].lexema == LEX_LITERAL)
			{
				if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
				}
				else
				{
					(*fout.stream) << "\n\tpush " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
				}
			}
			else if (lex.lextable.table[j].lexema == LEX_ID)
			{
				if (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype == IT::STR)
				{
					(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				}
				else
				{
					(*fout.stream) << "\n\tpush " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				}
			}
		}

		if (lex.lextable.table[j].idxTI != TI_NULLIDX && lex.idtable.table[lex.lextable.table[j].idxTI].idtype == IT::F)
		{
			break;
		}
		pos++;
	}
	return pos;
}

int GEN::ProcessArithmeticOperations(LA::LEX lex, Out::OUT& fout, int pos, bool isAssignment, int curLine, int column)
{
	bool isFunctionCalled = false;
	for (int j = pos + 1; lex.lextable.table[j].lexema != LEX_SEMICOLON; ++j)
	{
		if (lex.lextable.table[j].sn != curLine) {
			curLine = lex.lextable.table[j].sn;
			column = 1;
		}
		column++;

		if (lex.lextable.table[j].lexema == LEX_PLUS ||
			lex.lextable.table[j].lexema == LEX_MINUS ||
			lex.lextable.table[j].lexema == LEX_MULTIPLY ||
			lex.lextable.table[j].lexema == LEX_DIVISION ||
			lex.lextable.table[j].lexema == LEX_PERCENT)
		{
			(*fout.stream) << "\n\t; Получаем из стека два значения";
			(*fout.stream) << "\n\tpop ebx";
			if (!isFunctionCalled) 
			{
				(*fout.stream) << "\n\tpop eax";
				isFunctionCalled = false;
			}

			(*fout.stream) << "\n";

			switch (lex.lextable.table[j].lexema)
			{
			case LEX_PLUS:
				(*fout.stream) << "\n\t; Сложение";
				(*fout.stream) << "\n\tadd eax, ebx";
				break;
			case LEX_MINUS:
				(*fout.stream) << "\n\t; Вычитание";
				(*fout.stream) << "\n\tsub eax, ebx";
				break;
			case LEX_MULTIPLY:
				(*fout.stream) << "\n\t; Умножение";
				(*fout.stream) << "\n\timul eax, ebx";	
				break;
			case LEX_DIVISION:
				(*fout.stream) << "\n\t; Деление";
				(*fout.stream) << "\n\txor edx, edx";
				(*fout.stream) << "\n\tidiv ebx";
				break;
			case LEX_PERCENT:
				(*fout.stream) << "\n\t; Остаток от деления";
				(*fout.stream) << "\n\txor edx, edx";
				(*fout.stream) << "\n\tidiv ebx";
				(*fout.stream) << "\n\tmov eax, edx";
				break;
			}

			(*fout.stream) << "\n\tpush eax";
			(*fout.stream) << "\n";
		}
		else if (lex.lextable.table[j].lexema == '~')
		{
			throw ERROR_THROW_IN(333, curLine, column);
		}
		else if (lex.lextable.table[j].idxTI != TI_NULLIDX && lex.idtable.table[lex.lextable.table[j].idxTI].idtype != IT::F && (lex.lextable.table[j].lexema == LEX_LITERAL || lex.lextable.table[j].lexema == LEX_ID))
		{
			if (lex.lextable.table[j].lexema == LEX_LITERAL)
			{
				(*fout.stream) << "\n\tpush " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
			}
			else
			{
				(*fout.stream) << "\n\tpush " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
			}
		}

	}
	if (isAssignment)
	{
		(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[pos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[pos - 1].idxTI].scope << ", eax";
	}
	return pos;
}

int GEN::ProcessLogicalOperations(LA::LEX lex, Out::OUT& fout, int pos, bool isAssignment, int curLine, int column)
{
	IT::Entry lastOperandId;
	int lastOperandLiteral;
	bool isLastLiteral = false;
	bool isFirstOperandProcessed = false;

	(*fout.stream) << "\n\t; Обрабатываем логическую операцию";
	for (int j = pos + 1; lex.lextable.table[j].lexema != LEX_SEMICOLON; ++j)
	{
		if (lex.lextable.table[j].sn != curLine) {
			curLine = lex.lextable.table[j].sn;
			column = 1;
		}
		column++;

		if (lex.lextable.table[j].lexema == LEX_ID && lex.lextable.table[j + 1].lexema == LEX_ID)
		{
			if (lex.lextable.table[j].idxTI != TI_NULLIDX && lex.lextable.table[j + 1].idxTI != TI_NULLIDX &&
				lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype == IT::STR && lex.idtable.table[lex.lextable.table[j + 1].idxTI].iddatatype == IT::STR)
			{
				switch (lex.lextable.table[j + 2].lexema)
				{
				case LEX_MORE:
					(*fout.stream) << "\n\tpush 1";
					break;
				case LEX_LESS:
					(*fout.stream) << "\n\tpush 2";
					break;
				case LEX_MORE_EQUAL:
					(*fout.stream) << "\n\tpush 3";
					break;
				case LEX_LESS_EQUAL:
					(*fout.stream) << "\n\tpush 4";
					break;
				case LEX_EQUAL:
					(*fout.stream) << "\n\tpush 5";
					break;
				case LEX_NOT_EQUAL:
					(*fout.stream) << "\n\tpush 6";
					break;
				default:
					break;
				}
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j + 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j + 1].idxTI].scope;
				(*fout.stream) << "\n\tpush offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[j].idxTI].scope;
				(*fout.stream) << "\n\tcall compareCAY";
				break;
			}
		}

		switch (lex.lextable.table[j].lexema)
		{
		case LEX_LITERAL:
			switch (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype)
			{
			case IT::STR:
				lastOperandLiteral = lex.idtable.table[lex.lextable.table[j].idxTI].value.vstr.len;
				break;
			case IT::INT:
				lastOperandLiteral = lex.idtable.table[lex.lextable.table[j].idxTI].value.vint;
				break;
			case IT::BOOL:
				lastOperandLiteral = lex.idtable.table[lex.lextable.table[j].idxTI].value.vbool;
				break;
			default:
				break;
			}
			
			if (!isFirstOperandProcessed)
			{
				switch (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype)
				{
				case IT::STR:
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].value.vstr.len;
					break;
				case IT::INT:
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].value.vint;
					break;
				case IT::BOOL:
					(*fout.stream) << "\n\tmov eax, " << lex.idtable.table[lex.lextable.table[j].idxTI].value.vbool;
					break;
				default:
					break;
				}
			}
			else
			{
				isLastLiteral = true;
			}
			isFirstOperandProcessed = true;
			break;
		case LEX_ID:
			lastOperandId = lex.idtable.table[lex.lextable.table[j].idxTI];
			
			if (!isFirstOperandProcessed)
			{
				(*fout.stream) << "\n\tmov eax, " << lastOperandId.id + (string)"_" + lastOperandId.scope;
			}
			
			isFirstOperandProcessed = true;
			break;
		case '~':
			throw ERROR_THROW_IN(339, curLine, column);
		case LEX_MORE:
		case LEX_LESS:
		case LEX_MORE_EQUAL:
		case LEX_LESS_EQUAL:
		case LEX_EQUAL:
		case LEX_NOT_EQUAL:
			if(isLastLiteral)
				(*fout.stream) << "\n\tcmp eax, " << lastOperandLiteral;
			else
			{
				(*fout.stream) << "\n\tcmp eax, " << lastOperandId.id + (string)"_" + lastOperandId.scope;
			}

			if (lex.lextable.table[j].lexema == LEX_MORE)
				(*fout.stream) << "\n\tsetg al";
			else if (lex.lextable.table[j].lexema == LEX_LESS)
				(*fout.stream) << "\n\tsetl al";
			else if (lex.lextable.table[j].lexema == LEX_MORE_EQUAL)
				(*fout.stream) << "\n\tsetge al";
			else if (lex.lextable.table[j].lexema == LEX_LESS_EQUAL)
				(*fout.stream) << "\n\tsetle al";
			else if (lex.lextable.table[j].lexema == LEX_EQUAL)
				(*fout.stream) << "\n\tsete al";
			else if (lex.lextable.table[j].lexema == LEX_NOT_EQUAL)
				(*fout.stream) << "\n\tsetne al";

			(*fout.stream) << "\n\tmovzx eax, al";
			isFirstOperandProcessed = false;
			isLastLiteral = false;

			break;
		default:
			break;
		}
	}
	if (isAssignment)
	{
		(*fout.stream) << "\n\tmov " << lex.idtable.table[lex.lextable.table[pos - 1].idxTI].id + (string)"_" + lex.idtable.table[lex.lextable.table[pos - 1].idxTI].scope << ", eax";
	}
	(*fout.stream) << "\n";
	return pos;
}