#include "stdafx.h"

In::IN In::getin(wchar_t infile[])
{
    IN in;
    in.size = 0;
    in.lines = 0;
    in.ignore = 0;
    int position = 0;

    std::ifstream inputFile(infile);

    if (!inputFile.is_open())
    {
        throw ERROR_THROW(110);
    }

    char* buffer = new char[IN_MAX_LEN_TEXT];
    char inputChar = ' ';
    bool insideString = false;
    char prevChar = '\0';

    inputChar = inputFile.get();
    while (in.size < IN_MAX_LEN_TEXT)
    {
        if (inputFile.eof())
        {
            buffer[in.size] = '\0';
            break;
        }

        if ((inputChar == IN_CODE_ENDL || inputChar == '\r') && in.code[inputChar] != IN::F)
        {
            position = -1;
            in.lines++;

            if (inputChar == '\r' && inputFile.peek() == IN_CODE_ENDL)
            {
                inputChar = inputFile.get();
            }
        }

        if (inputChar == '"')
        {
            insideString = !insideString;
        }

        switch (in.code[(unsigned char)inputChar])
        {
        case IN::T:
            position++;
            if (insideString)
            {
                buffer[in.size++] = inputChar;
            }
            else
            {
                if (inputChar == IN_CODE_SPACE || inputChar == IN_CODE_TAB)
                {
                    if (std::isalpha(prevChar) && std::isalpha(inputFile.peek()) ||
                        std::isalnum(prevChar) && std::isalnum(inputFile.peek()) ||
                        std::isalpha(prevChar) && inputFile.peek() == '\"' ||
                        std::isalnum(prevChar) && inputFile.peek() == '\"')
                    {
                        buffer[in.size++] = IN_CODE_SPACE;
                    }
                    else
                    {
                        inputChar = inputFile.get();
                        continue;
                    }
                }
                else
                {
                    buffer[in.size++] = inputChar;
                }
            }
            break;

        case IN::F:
            buffer[in.size] = '\0';
            throw ERROR_THROW_IN(111, in.lines, position);
            break;

        case IN::I:
            in.ignore++;
            break;

        default:
            buffer[in.size] = in.code[(unsigned char)inputChar];
            position++;
            in.size++;
        }

        prevChar = inputChar;
        inputChar = inputFile.get();
    }

    if (!inputFile.eof())
    {
        throw ERROR_THROW(113);
    }

    in.text = new char[in.size];
    strncpy(in.text, buffer, in.size);
    in.text[in.size] = '\0';

    delete[] buffer;
    inputFile.close();

    return in;
}
