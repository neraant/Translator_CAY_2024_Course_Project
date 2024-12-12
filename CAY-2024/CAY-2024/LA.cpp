#include "stdafx.h"

bool trueFlag = false;
bool falseFlag = false;
bool boolFlag = false;
bool intFlag = false;
bool floatFlag = false;
bool stringFlag = false;
bool declareFunctionFlag = false;
bool declareVariableFlag = false;
bool declareIfFlag = false;
bool declareWhileFlag = false;
bool declareElseFlag = false;
int quotes = 0;

int scopeCounter = 0;

LA::LEX LA::LA(Parm::PARM, In::IN in)
{
	LEX lexresult;
	LT::LexTable lextable = LT::Create(LT_MAXSIZE);
	IT::IdTable idtable = IT::Create(TI_MAXSIZE);
	LT::Entry currentEntryLT = LT::Entry();
	IT::Entry currentEntryIT = IT::Entry();
	char* buffer = new char[LA_MAXSIZE];
	char lexema = NULL;
	int indexIT;
	int bufferIndex = 0;
	int numberOfLiterals = 0;
	int currentLine = 1;
	int column = 0;

	vector<string> scopeStack;
	string global = "global\0";
	scopeStack.push_back(global);

	std::vector<char> separators = { ';' ,' ', ',', '[', ']', '(', ')', '{', '}', '+', '-', '*', '/', '%', '>', '<', '=', '!', '|'};

	for (int i = 0; i < in.size; i++)
	{
		if (find(separators.begin(), separators.end(), in.text[i]) == separators.end())
		{
			if (in.text[i] == '\"' && lexema == NULL)
			{
				quotes++;

				buffer[bufferIndex++ ] = in.text[i++];
				if (in.text[i] != LA_NEW_LINE) {
					column++;
				}
				while (in.text[i] != '\"' && i < in.size)
				{
					buffer[bufferIndex++] = in.text[i++];
					if (in.text[i] != LA_NEW_LINE) {
						column++;
					}
				}
				if (i < in.size && in.text[i] == '\"') {
					buffer[bufferIndex++] = in.text[i];
					quotes--;
				}
				else {
					throw ERROR_THROW_IN(305, currentLine, column);
				}
				continue;
			}
			buffer[bufferIndex++] = in.text[i];

			if (bufferIndex > LA_MAXSIZE)
			{
				throw ERROR_THROW(126);
			}
		}
		else
		{
			buffer[bufferIndex] = '\0';
			lexema = FST(buffer);

			if (lexema == LEX_MAIN)
			{
				strncpy(currentEntryIT.id, buffer, ID_MAXSIZE);
				currentEntryIT.id[strlen(buffer)] = '\0';
				currentEntryIT.idtype = IT::F;
				currentEntryIT.iddatatype = IT::VOID;
				currentEntryIT.idxfirstLE = currentLine;

				if (!scopeStack.empty())
				{
					strncpy(currentEntryIT.scope, scopeStack.back().c_str(), strlen(scopeStack.back().c_str()));
					currentEntryIT.scope[strlen(scopeStack.back().c_str())] = '\0';
				}
				else
				{
					currentEntryIT.scope[0] = NULL;
				}

				indexIT = IT::Search(idtable, currentEntryIT);
				if (indexIT >= 0)
				{
					throw ERROR_THROW_IN(321, currentLine, column);
				}
				else
				{
					currentEntryLT.idxTI = idtable.size;
					IT::Add(idtable, currentEntryIT);
					currentEntryIT = IT::Entry();
				}
			}

			if (lexema == LEX_LITERAL)
			{
				currentEntryIT.idtype = IT::L;

				if (in.text[i - strlen(buffer)] == '\"' && in.text[i - 1] == '\"')
				{
					currentEntryIT.iddatatype = IT::STR;
					int bufferLength = strlen(buffer) - 2;
					int length = bufferLength > TI_STR_MAXSIZE ? TI_STR_MAXSIZE : bufferLength;
					strncpy(currentEntryIT.value.vstr.str, buffer + 1, length);
					currentEntryIT.value.vstr.str[length] = '\0';
					currentEntryIT.value.vstr.len = length;
				}

				if ((isdigit((in.text[i - strlen(buffer)])) || (in.text[i] >= 'A' && in.text[i] <= 'F')))
				{
					int intLiteral = 0;

					if (strcmp(buffer, "0") == 0)
					{
						intLiteral = 0;
					}
					else {
						bool isNegative = (in.text[i - strlen(buffer) - 1] == LA_MINUS &&
							(in.text[i - strlen(buffer) - 2] == '(' ||
								in.text[i - strlen(buffer) - 2] == '+' ||
								in.text[i - strlen(buffer) - 2] == '-' ||
								in.text[i - strlen(buffer) - 2] == '*' ||
								in.text[i - strlen(buffer) - 2] == '/' ||
								in.text[i - strlen(buffer) - 2] == '=' ||
								in.text[i - strlen(buffer) - 2] == '{'));


						if (isNegative)
						{
							string bufferWithMinus = "-" + string(buffer);

							if (bufferWithMinus[2] == 'b')
							{
								string binaryString = bufferWithMinus.substr(3);
								intLiteral = stoi(binaryString, nullptr, 2);
							}
							else if (bufferWithMinus[2] == 'x')
							{
								string binaryString = bufferWithMinus.substr(3);
								intLiteral = stoi(binaryString, nullptr, 16);
							}
							else
							{
								intLiteral = stoi(buffer);
							}
						}
						else
						{
							if (buffer[1] == 'b')
							{
								string binaryString = string(buffer).substr(2);
								intLiteral = stoi(binaryString, nullptr, 2);

							}
							else if (buffer[1] == 'x')
							{
								string binaryString = string(buffer).substr(2);
								intLiteral = stoi(binaryString, nullptr, 16);
							}
							else
							{
								intLiteral = stoi(buffer);
							}
						}

						if (isNegative)
						{
							intLiteral = -intLiteral;
							lextable.size--;
						}
					}

					if (intLiteral >= TI_INT_MINSIZE && intLiteral <= TI_INT_MAXSIZE)
					{
						currentEntryIT.iddatatype = IT::INT;
						currentEntryIT.value.vint = intLiteral;
					}
					else
					{
						currentEntryIT.iddatatype = IT::INT;
						currentEntryIT.value.vint = 0;
					}
				}

				if (trueFlag)
				{
					currentEntryIT.iddatatype = IT::BOOL;
					currentEntryIT.value.vbool = true;
					trueFlag = false;
				}

				if (falseFlag)
				{
					currentEntryIT.iddatatype = IT::BOOL;
					currentEntryIT.value.vbool = false;
					falseFlag = false;
				}

				indexIT = IT::Search(idtable, currentEntryIT);

				if (indexIT >= 0)
				{
					currentEntryLT.idxTI = indexIT;
				}
				else
				{
					sprintf(currentEntryIT.id, "L%d", numberOfLiterals);

					if (!scopeStack.empty())
					{
						strncpy(currentEntryIT.scope, scopeStack.back().c_str(), ID_MAXSIZE);
						currentEntryIT.scope[strlen(scopeStack.back().c_str())] = '\0';
					}
					else
					{
						currentEntryIT.scope[0] = NULL;
					}

					currentEntryLT.idxTI = idtable.size;
					currentEntryIT.idxfirstLE = currentLine;
					IT::Add(idtable, currentEntryIT);
					currentEntryIT = IT::Entry();
					numberOfLiterals++;
				}
			}

			if (lexema == LEX_ID)
			{
				bool addedToITFlag = false;
				currentEntryLT.lexema = lexema;
				currentEntryIT.idtype = IT::V;
				strncpy(currentEntryIT.id, buffer, ID_MAXSIZE);
				currentEntryIT.id[strlen(buffer)] = '\0';
				if (!scopeStack.empty())
				{
					strncpy(currentEntryIT.scope, scopeStack.back().c_str(), strlen(scopeStack.back().c_str()));
					currentEntryIT.scope[strlen(scopeStack.back().c_str())] = '\0';
				}
				else
				{
					currentEntryIT.scope[0] = NULL;
				}

				if (strcmp(buffer, DATE_FUNC) == 0 || strcmp(buffer, ISEVEN_FUNC) == 0 || strcmp(buffer, ISEMPTY_FUNC) == 0
					|| strcmp(buffer, RAND_FUNC) == 0 || strcmp(buffer, LENGTH_FUNC) == 0)
				{
					currentEntryIT.idtype = IT::F;

					if (strcmp(buffer, DATE_FUNC) == 0)
					{
						currentEntryIT.iddatatype = IT::STR;
						currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
						currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
					}

					if (strcmp(buffer, ISEVEN_FUNC) == 0 || strcmp(buffer, ISEMPTY_FUNC) == 0)
					{
						currentEntryIT.iddatatype = IT::BOOL;
						currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
					}

					if (strcmp(buffer, RAND_FUNC) == 0 || strcmp(buffer, LENGTH_FUNC) == 0)
					{
						currentEntryIT.iddatatype = IT::INT;
						currentEntryIT.value.vint = TI_INT_DEFAULT;
					}

					currentEntryLT.idxTI = idtable.size;
					currentEntryIT.idxfirstLE = currentLine;
					IT::Add(idtable, currentEntryIT);
					currentEntryIT = IT::Entry();
					addedToITFlag = true;
				}

				if (lextable.table[lextable.size - 2].lexema == LEX_VAR && declareVariableFlag)
				{
					if (lextable.table[lextable.size - 1].lexema == LEX_INTEGER && intFlag)
					{
						currentEntryIT.iddatatype = IT::INT;
						currentEntryIT.value.vint = TI_INT_DEFAULT;
						intFlag = false;
					}
					if (lextable.table[lextable.size - 1].lexema == LEX_STRING && stringFlag)
					{
						currentEntryIT.iddatatype = IT::STR;
						currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
						currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
						stringFlag = false;
					}
					if (lextable.table[lextable.size - 1].lexema == LEX_BOOL && boolFlag)
					{
						currentEntryIT.iddatatype = IT::BOOL;
						currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
						boolFlag = false;
					}

					indexIT = Search(idtable, currentEntryIT);
					if (indexIT >= 0)
					{
						throw ERROR_THROW_IN(303, currentLine, column);
					}
					else
					{
						currentEntryLT.idxTI = idtable.size;
						currentEntryIT.idxfirstLE = currentLine;
						IT::Add(idtable, currentEntryIT);
						currentEntryIT = IT::Entry();
						addedToITFlag = true;
					}

					declareVariableFlag = false;
				}

				if (lextable.table[lextable.size - 1].lexema == LEX_FUNCTION && declareFunctionFlag)
				{
					currentEntryIT.idtype = IT::F;

					if (lextable.table[lextable.size - 2].lexema == LEX_INTEGER && intFlag)
					{
						currentEntryIT.iddatatype = IT::INT;
						currentEntryIT.value.vint = TI_INT_DEFAULT;
						intFlag = false;
					}
					if (lextable.table[lextable.size - 2].lexema == LEX_STRING && stringFlag)
					{
						currentEntryIT.iddatatype = IT::STR;
						currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
						currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
						stringFlag = false;
					}
					if (lextable.table[lextable.size - 2].lexema == LEX_BOOL && boolFlag)
					{
						currentEntryIT.iddatatype = IT::BOOL;
						currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
						boolFlag = false;
					}

					indexIT = Search(idtable, currentEntryIT);
					if (indexIT >= 0)
					{
						throw ERROR_THROW_IN(301, currentLine, column);
					}
					else
					{
						currentEntryLT.idxTI = idtable.size;
						currentEntryIT.idxfirstLE = currentLine;
						IT::Add(idtable, currentEntryIT);
						currentEntryIT = IT::Entry();
						addedToITFlag = true;
					}
				}

				if ((lextable.table[lextable.size - 2].lexema == LA_LEFTTHESIS &&
					lextable.table[lextable.size - 3].lexema == LEX_ID &&
					lextable.table[lextable.size - 3].idxTI == idtable.size - 1 &&
					idtable.table[idtable.size - 1].idtype == IT::F) ||
					lextable.table[lextable.size - 2].lexema == LA_COMMA && idtable.table[idtable.size - 1].idtype == IT::P)
				{
					currentEntryIT.idtype = IT::P;

					if (lextable.table[lextable.size - 1].lexema == LEX_INTEGER && intFlag)
					{
						currentEntryIT.iddatatype = IT::INT;
						currentEntryIT.value.vint = TI_INT_DEFAULT;
						intFlag = false;
					}
					if (lextable.table[lextable.size - 1].lexema == LEX_BOOL && boolFlag)
					{
						currentEntryIT.iddatatype = IT::BOOL;
						currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
						boolFlag = false;
					}
					if (lextable.table[lextable.size - 1].lexema == LEX_STRING && stringFlag)
					{
						currentEntryIT.iddatatype = IT::STR;
						currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
						currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
						stringFlag = false;
					}

					indexIT = Search(idtable, currentEntryIT);
					if (indexIT >= 0)
					{
						throw ERROR_THROW_IN(302, currentLine, column);
					}
					else
					{
						currentEntryLT.idxTI = idtable.size;
						currentEntryIT.idxfirstLE = currentLine;
						IT::Add(idtable, currentEntryIT);
						currentEntryIT = IT::Entry();
						addedToITFlag = true;
					}
				}

				if (!addedToITFlag)
				{
					int indexIT = Search(idtable, currentEntryIT);

					if (indexIT >= 0)
					{
						currentEntryLT.idxTI = indexIT;
						currentEntryIT = IT::Entry();
					}
					else
					{
						for (int i = scopeStack.size() - 2; i >= 0; --i)
						{
							strncpy(currentEntryIT.scope, scopeStack[i].c_str(), strlen(scopeStack[i].c_str()));
							currentEntryIT.scope[strlen(scopeStack[i].c_str())] = '\0';
							indexIT = Search(idtable, currentEntryIT);

							if (indexIT >= 0)
							{
								break;
							}
						}

						currentEntryLT.idxTI = indexIT;
						currentEntryIT = IT::Entry();
					}

					if (indexIT < 0)
					{
						throw ERROR_THROW_IN(311, currentLine, i);
					}
				}
			}

			if ((lexema == LEX_RIGHTPAREN || lexema == LEX_SEMICOLON) && quotes != 0)
				throw ERROR_THROW_IN(305, currentLine, column);

			if (lexema == NULL && buffer[0] != '\0' &&
				std::find(separators.begin(), separators.end(), in.text[i]) == separators.end())
			{
				throw ERROR_THROW_IN(127, currentLine, column);
			}
				
			bufferIndex = 0;
		}

		if (lexema != NULL)
		{
			currentEntryLT.lexema = lexema;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			lexema = NULL;
			currentEntryLT = LT::Entry();
		}

		switch (in.text[i])
		{
		case LA_NEW_LINE:
			currentLine++;
			column = 0;
			currentEntryLT = LT::Entry();
			break;
		case LA_SEMICOLON:
			currentEntryLT.lexema = LEX_SEMICOLON;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case IN_CODE_SPACE:
			column++;
			break;
		case LEX_LEFTPAREN:
			currentEntryLT.lexema = LEX_LEFTPAREN;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);

			if (declareFunctionFlag)
			{
				for (int i = idtable.size - 1; i >= 0; i--)
				{
					if (idtable.table[i].idtype == IT::F)
					{
						scopeStack.push_back(idtable.table[i].id);
						break;
					}
				}
			}
			if (declareIfFlag || declareWhileFlag)
			{
				char scope[ID_MAXSIZE];
				if (declareIfFlag)
				{
					sprintf(scope, "if_scope%d", scopeCounter);
				}
				if (declareWhileFlag)
				{
					sprintf(scope, "while_scope%d", scopeCounter);
				}
				scopeStack.push_back(scope);
			}

			currentEntryLT = LT::Entry();
			break;
		case LEX_RIGHTPAREN:
			currentEntryLT.lexema = LEX_RIGHTPAREN;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);

			if (!scopeStack.empty() && declareFunctionFlag)
			{
				scopeStack.pop_back();
				declareFunctionFlag = false;
			}
			if (!scopeStack.empty() && (declareIfFlag || declareElseFlag || declareWhileFlag))
			{
				scopeStack.pop_back();
			}

			currentEntryLT = LT::Entry();
			break;
		case LEX_LEFTBRACE: {
			currentEntryLT.lexema = LEX_LEFTBRACE;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);

			char scope[ID_MAXSIZE] = "";

			if (!declareWhileFlag && !declareElseFlag && !declareIfFlag) {
				for (int i = idtable.size - 1; i >= 0; i--) {
					if (idtable.table[i].idtype == IT::F) {
						strncpy(scope, idtable.table[i].id, ID_MAXSIZE - 1);
						scope[ID_MAXSIZE - 1] = '\0';
						scopeStack.push_back(scope);

						break;
					}
				}
			}
			else
			{
				if (declareIfFlag) {
					sprintf(scope, "if_scope%d", scopeCounter);
					declareIfFlag = false;
				}
				else if (declareElseFlag) {
					sprintf(scope, "else_scope%d", scopeCounter);
					declareElseFlag = false;
				}
				else if (declareWhileFlag) {
					sprintf(scope, "while_scope%d", scopeCounter);
					declareWhileFlag = false;
				}
			
				if (scope[0] != '\0') {
					scopeStack.push_back(scope);
				}
			}
			scopeCounter++;

			currentEntryLT = LT::Entry();
			break;
		}
		case LEX_RIGHTBRACE: {
			currentEntryLT.lexema = LEX_RIGHTBRACE;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);

			if (!scopeStack.empty())
			{
				scopeStack.pop_back();
			}

			currentEntryLT = LT::Entry();
			break;
		}
		case LA_COMMA:
			currentEntryLT.lexema = LEX_COMMA;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_PLUS:
			currentEntryLT.lexema = LEX_PLUS;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_MINUS:
			currentEntryLT.lexema = LEX_MINUS;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_MULTIPLY:
			currentEntryLT.lexema = LEX_MULTIPLY;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_DIVISION:
			currentEntryLT.lexema = LEX_DIVISION;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_PERCENT:
			currentEntryLT.lexema = LEX_PERCENT;
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_EQUAL:
			currentEntryLT.lexema = LEX_EQUAL_SIGN;
			if (in.text[i + 1] == LA_EQUAL) {
				currentEntryLT.lexema = LEX_EQUAL;
				i++;
			}
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_MORE:
			currentEntryLT.lexema = LEX_MORE;
			if (in.text[i + 1] == LA_EQUAL) {
				currentEntryLT.lexema = LEX_MORE_EQUAL;
				i++;
			}
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_LESS:
			currentEntryLT.lexema = LEX_LESS;
			if (in.text[i + 1] == LA_EQUAL) {
				currentEntryLT.lexema = LEX_LESS_EQUAL; 
				i++;
			}
			currentEntryLT.sn = currentLine;
			LT::Add(lextable, currentEntryLT);
			currentEntryLT = LT::Entry();
			break;
		case LA_NOT:
			if (in.text[i + 1] == LA_EQUAL)
			{
				currentEntryLT.lexema = LEX_NOT_EQUAL;
				i++;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
			}
			else
			{
				throw ERROR_THROW_IN(206, currentLine, column);
			}
			currentEntryLT = LT::Entry();
			break;
		}
	}

	lexresult.lextable = lextable;
	lexresult.idtable = idtable;

	LT::WriteTable(lextable);
	IT::WriteTable(idtable);

	delete[] buffer;
	return lexresult;
}

char LA::FST(char* str)
{
	FST_MAIN;
	FST_INT;
	FST_STR;
	FST_BOOL;
	FST_TRUE;
	FST_FALSE;
	FST_IDENTIFIER;
	FST_FUNCTION;
	FST_VARIABLE;
	FST_RETURN;

	FST_WHILE;
	FST_IF;
	FST_ELSE;

	FST_WRITE;
	FST_WRITELINE;
	FST_DATE;
	FST_LEN;
	FST_ISEMPTY;
	FST_ISEVEN;
	FST_RAND;

	FST_INT_BIN_LITERAL;
	FST_HEX_BIN_LITERAL;
	FST_INT_LITERAL;
	FST_STRING_LITERAL;

	if (FST::execute(_main)) {
		return LEX_MAIN;
	}
	if (FST::execute(_int_bin_literal)) {
		return LEX_LITERAL;
	}
	if (FST::execute(_int_hex_literal)) {
		return LEX_LITERAL;
	}
	if (FST::execute(_int_literal)) {
		return LEX_LITERAL;
	}
	if (FST::execute(_string_literal)) {
		return LEX_LITERAL;
	}
	if (FST::execute(_int)) {
		intFlag = true;
		return LEX_INTEGER;
	}
	if (FST::execute(_string)) {
		stringFlag = true;
		return LEX_STRING;
	}
	if (FST::execute(_true)) {
		trueFlag = true;
		return LEX_LITERAL;
	}
	if (FST::execute(_false)) {
		falseFlag = true;
		return LEX_LITERAL;
	}
	if (FST::execute(_bool)) {
		boolFlag = true;
		return LEX_BOOL;
	}
	if (FST::execute(_variable))
	{
		declareVariableFlag = true;
		return LEX_VAR;
	}
	if (FST::execute(_function))
	{
		declareFunctionFlag = true;
		return LEX_FUNCTION;
	}
	if (FST::execute(_return))
	{
		return LEX_RETURN;
	}
	if (FST::execute(_while))
	{
		declareWhileFlag = true;
		return LEX_WHILE;
	}
	if (FST::execute(_if))
	{
		declareIfFlag = true;
		return LEX_IF;
	}
	if (FST::execute(_else))
	{
		declareElseFlag = true;
		return LEX_ELSE;
	}
	if (FST::execute(_write))
	{
		return LEX_WRITE;
	}
	if (FST::execute(_writeline))
	{
		return LEX_WRITELINE;
	}
	if (FST::execute(_identifier))
	{
		return LEX_ID;
	}
}