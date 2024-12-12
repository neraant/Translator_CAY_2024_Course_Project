#pragma once
#include "stdafx.h"

namespace Out
{
	struct OUT
	{
		wchar_t outfile[PARM_MAX_SIZE];
		ofstream* stream;
	};

	static const OUT INITOUT{ L"",NULL };
	void WriteOut(OUT out, In::IN in);
	void WriteError(OUT out, Error::ERROR error);
	void Close(OUT out);
	OUT GetOut(wchar_t outfile[]);
}
