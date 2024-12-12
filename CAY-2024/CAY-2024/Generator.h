#pragma once
#include "stdafx.h"

#define HEAD								\
".586P\n"									\
".MODEL FLAT, STDCALL\n"					\
"includelib libucrt.lib\n"					\
"includelib kernel32.lib\n"					\
"includelib ../Debug/CAY-2024ASMLIB.lib\n"	\
"includelib ../Debug/CAY-2024LIB.lib\n"

#define PROTOTYPES							\
"\nExitProcess PROTO : DWORD"				\
"\ncopy_string PROTO"						\
"\nextrn WriteInt : proc"					\
"\nextrn WriteLineInt : proc"				\
"\nextrn WriteStr : proc"					\
"\nextrn WriteLineStr : proc"				\
"\nextrn WriteBool : proc"					\
"\nextrn WriteLineBool : proc"				\
"\nextrn dateCAY : proc"					\
"\nextrn randCAY : proc"					\
"\nextrn isEvenCAY : proc"					\
"\nextrn isEmptyCAY : proc"					\
"\nextrn lenCAY : proc"						\
"\nextrn compareCAY : proc"					\
"\n\n.STACK 4096\n\n"

#define FOOTER								\
"\n\tpush 0"								\
"\n\tcall ExitProcess"						\
"\nmain ENDP\n\nend main"

namespace GEN
{
	void AsmGeneration(LA::LEX lex, Out::OUT& fout);
	void GenerateHead(LA::LEX lex, Out::OUT& fout);
	void GenerateConst(LA::LEX lex, Out::OUT& fout);
	void GenerateData(LA::LEX lex, Out::OUT& fout);
	void GenerateCode(LA::LEX lex, Out::OUT& fout);
	void GenerateFooter(LA::LEX lex, Out::OUT& fout);
	int ProcessFunction(LA::LEX lex, Out::OUT& fout, int i, int curLine, int column, IT::Entry& funcIT, bool& isFunc, vector<IT::Entry>& paramsContainer);
	int ProcessIf(LA::LEX lex, Out::OUT& fout, int i, int curLine, int column, bool& isIf, bool& isElse, string& ifScopeName, stack<string>& scopeStack, int& ifScopeCounter);
	int ProcessWhile(LA::LEX lex, Out::OUT& fout, int i, int curLine, int column, bool& isWhile, string& whileScopeName, stack<string>& scopeStack, int& whileScopeCounter, string& counterName);
	int ProcessEqual(LA::LEX lex, Out::OUT& fout, bool& isArOperations, bool& isLogicalOperations, int i, int curLine, int column);
	int ProcessWrite(LA::LEX lex, Out::OUT& fout, bool& isArOperations, bool& isLogicalOperations, int i, int curLine, int column, string codeLexema);
	int ProcessReturn(LA::LEX lex, Out::OUT& fout, int i, bool& isReturned);
	int ProcessRightbrace(LA::LEX lex, Out::OUT& fout, int i, bool& isFunc, bool& isReturned, bool& isElse, bool& isIf, bool& isWhile, IT::Entry& funcIT, stack<string>& scopeStack, string& ifScopeName, string& whileScopeName, string& counterName);
	int GetFuncParams(LA::LEX lex, Out::OUT& fout, int pos);
	int ProcessArithmeticOperations(LA::LEX lex, Out::OUT& fout, int pos, bool isAssignment, int curLine, int column);
	int ProcessLogicalOperations(LA::LEX lex, Out::OUT& fout, int pos, bool isAssignment, int curLine, int column);
}