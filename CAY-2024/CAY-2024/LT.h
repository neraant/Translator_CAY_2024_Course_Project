#pragma once

#define LT_MAXSIZE				4096
#define	LT_TI_NULLXDX			0XFFFFFFFF
#define LEX_INTEGER				't'
#define LEX_FLOAT				't'
#define LEX_STRING				't'
#define LEX_BOOL				't'
#define LEX_ID					'i'
#define LEX_LITERAL				'l'
#define LEX_FUNCTION			'f'
#define LEX_MAIN				'm'
#define LEX_WHILE				'w'
#define LEX_IF					'z'
#define LEX_ELSE				'e'
#define LEX_VAR					'v'
#define LEX_RETURN				'r'

#define LEX_SEMICOLON			';'
#define LEX_COMMA				','

#define LEX_LEFTBRACE			'{'
#define LEX_RIGHTBRACE			'}'
#define LEX_LEFTPAREN			'('
#define LEX_RIGHTPAREN			')'

#define LEX_PLUS				'+'
#define LEX_MINUS				'-'
#define	LEX_MULTIPLY			'*'			
#define	LEX_DIVISION			'/'
#define LEX_PERCENT				'%'

#define LEX_MORE				'>'
#define LEX_LESS				'<'
#define LEX_EQUAL				'&'
#define LEX_NOT_EQUAL			'j'
#define LEX_MORE_EQUAL			'x'
#define LEX_LESS_EQUAL			'k'
#define	LEX_EQUAL_SIGN			'='

#define DATE_FUNC				"date"
#define RAND_FUNC				"rand"
#define ISEVEN_FUNC				"isEven"
#define ISEMPTY_FUNC			"isEmpty"
#define LENGTH_FUNC				"len"
#define LEX_WRITE				'p'
#define LEX_WRITELINE			's'

#define LT_FILENAME				"C:\\Users\\chere\\OneDrive\\Рабочий стол\\КП\\CAY-2024\\Debug\\LT.txt"


namespace LT
{
	struct Entry
	{
		char lexema;
		int sn;
		int idxTI = LT_TI_NULLXDX;

		Entry();
		Entry(char lexema, int sn, int idxTI);
	};

	struct LexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void WriteTable(LexTable& lextable);
	void Delete(LexTable& lextable);
};
