#pragma once

namespace SA {
	void SA(LT::LexTable lextable, IT::IdTable idtable);
	bool CheckConditionValuesType(const vector<LT::Entry>& values, IT::IdTable& idtable, IT::IDDATATYPE expectedType, char conditionLexema, int curLine, int Column);
	bool CheckCondition(IT::IdTable& idtable, LT::LexTable& lextable, int pos, char conditionLexema, int curLine, int Column);
	void CheckAssignment(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column);
	void IgnoreParentheses(LT::LexTable& lextable, int& pos, int& valueIdx, int curLine, int Column);
	void CheckFunction(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column);
	void CheckIdentifier(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column);
	void CheckMainRet(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column);
	void CheckRet(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, int curLine, int Column);
	void isNested(LT::LexTable& lextable, IT::IdTable& idtable, int& pos, char assignSymbol, int curLine, int Column);
}