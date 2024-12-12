#pragma once
#include "LT.h"
#include "IT.h"

#define LA_MAXSIZE		4096
#define LA_MARK			'\''
#define LA_NEW_LINE		'|'

#define LA_SEMICOLON	';'
#define LA_COMMA		','

#define LA_LEFTBRACE	'{'
#define LA_RIGHTBRACE	'}'
#define LA_LEFTTHESIS	'('
#define LA_RIGHTTHESIS	')'

#define LA_PLUS			'+'
#define LA_MINUS		'-'
#define LA_MULTIPLY		'*'
#define LA_EQUAL		'='
#define LA_DIVISION		'/'
#define LA_PERCENT		'%'

#define LA_MORE			'>'
#define LA_MORE_EQUAL	'>='
#define LA_LESS			'<'
#define LA_LESS_EQUAL	'<='
#define LA_NOT			'!'


namespace LA
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	 idtable;
	};

	char FST(char* str);
	LEX LA(Parm::PARM parm, In::IN in);
}