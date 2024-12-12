#include "stdafx.h"

namespace PN
{
    void PN(LT::LexTable& lextable, IT::IdTable& idtable)
    {
        for (int i = 0; i < lextable.size; ++i)
        {
            if (lextable.table[i].lexema == LEX_EQUAL_SIGN || lextable.table[i - 1].lexema == LEX_WRITELINE || lextable.table[i - 1].lexema == LEX_WRITE)
            {
                if  (lextable.table[i + 2].lexema == LEX_SEMICOLON)
                {
                    continue;
                }
                else
                {
                      PolishNotation(++i, lextable, idtable, LEX_SEMICOLON);
                }
            }
            if (lextable.table[i].lexema == LEX_IF || lextable.table[i].lexema == LEX_WHILE)
            {
                i += 2;
                PolishNotation(i, lextable, idtable, LEX_RIGHTPAREN);
            }
        }
    }

    void PolishNotation(int pos, LT::LexTable& lextable, IT::IdTable& idtable, char endSymbol)
    {
        stack<LT::Entry> operatorsStack;
        queue<LT::Entry> outputQueue;
        int lexCounter = 0;

        for (int i = pos; lextable.table[i].lexema != endSymbol; ++i, lexCounter++)
        {
            LT::Entry currentLexema = lextable.table[i];

            if (lextable.table[i].idxTI != TI_NULLIDX && idtable.table[lextable.table[i].idxTI].idtype == IT::F)
            {
                operatorsStack.push(currentLexema);
            }
            else if (currentLexema.lexema == LEX_ID || currentLexema.lexema == LEX_LITERAL)
            {
                outputQueue.push(currentLexema);
            }
            else if (isOperationSign(currentLexema.lexema))
            {
                while (!operatorsStack.empty() && GetPriority(operatorsStack.top().lexema) >= GetPriority(currentLexema.lexema))
                {
                    outputQueue.push(operatorsStack.top());
                    operatorsStack.pop();
                }
                operatorsStack.push(currentLexema);
            }
            else if (currentLexema.lexema == LEX_LEFTPAREN)
            {
                if (lextable.table[i - 1].idxTI != -1 && idtable.table[lextable.table[i - 1].idxTI].idtype == IT::F)
                {
                    outputQueue.push(LT::Entry{ '~', lextable.table[i].sn, -1 });
                }
                operatorsStack.push(currentLexema);
            }
            else if (currentLexema.lexema == LEX_RIGHTPAREN)
            {
                while (!operatorsStack.empty() && operatorsStack.top().lexema != LEX_LEFTPAREN)
                {
                    outputQueue.push(operatorsStack.top());
                    operatorsStack.pop();
                }

                if (!operatorsStack.empty() && operatorsStack.top().lexema == LEX_LEFTPAREN)
                {
                    operatorsStack.pop(); 
                }
                
                if (!operatorsStack.empty() && operatorsStack.top().idxTI != TI_NULLIDX && idtable.table[operatorsStack.top().idxTI].idtype == IT::F)
                {
                    outputQueue.push(operatorsStack.top());
                    operatorsStack.pop();
                }
            }
            else if (currentLexema.lexema == LEX_COMMA)
            {
                while (!operatorsStack.empty() && operatorsStack.top().lexema != LEX_LEFTPAREN)
                {
                    outputQueue.push(operatorsStack.top());
                    operatorsStack.pop();
                }
            }
        }
        while (!operatorsStack.empty())
        {
            outputQueue.push(operatorsStack.top());
            operatorsStack.pop();
        }
        int i = 0;
        while (!outputQueue.empty()) 
        { 
            if (outputQueue.front().lexema == LEX_LEFTPAREN)
            {
                outputQueue.pop();
                continue;
            }
            lextable.table[pos + i] = outputQueue.front(); 
            outputQueue.pop(); 
            i++; 
        }
        while (i < lexCounter)
        {
            lextable.table[pos + i] = LT::Entry{ '|', lextable.table[pos].sn, -1 }; 
            i++;
        }
    }

    int GetPriority(char symbol)
    {
        switch (symbol)
        {
        case LEX_RIGHTPAREN:
        case LEX_LEFTPAREN:
            return 0;
        case LEX_COMMA:
            return 1;
        case LEX_EQUAL:
        case LEX_NOT_EQUAL:
            return 2;
        case LEX_LESS:
        case LEX_MORE:
        case LEX_LESS_EQUAL:
        case LEX_MORE_EQUAL:
            return 3;
        case LEX_PLUS:
        case LEX_MINUS:
            return 4;
        case LEX_MULTIPLY:
        case LEX_PERCENT:
        case LEX_DIVISION: 
            return 5;
        }

        return -1;
    }

    bool isOperationSign(char signSymbol)
    {
        return  signSymbol == LEX_DIVISION  || signSymbol == LEX_MULTIPLY   ||
                signSymbol == LEX_PLUS      || signSymbol == LEX_MINUS      ||
                signSymbol == LEX_MORE      || signSymbol == LEX_MORE_EQUAL ||
                signSymbol == LEX_LESS      || signSymbol == LEX_LESS_EQUAL ||
                signSymbol == LEX_PERCENT   || signSymbol == LEX_NOT_EQUAL  ||
                signSymbol == LEX_EQUAL;
    }
}