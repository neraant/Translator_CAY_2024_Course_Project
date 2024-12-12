#include "stdafx.h"


Parm::PARM Parm::getparm(int argc, wchar_t* argv[])
{
	Parm::PARM parm;
	bool inFlag = false, outFlag = false, logFlag = false;

	for (int i = 0; i < argc; i++)
	{
		if (wcslen(argv[i]) > PARM_MAX_SIZE)
		{
			throw ERROR_THROW(101);
		}
		if (wcsstr(argv[i], PARM_IN))
		{
			wcscpy_s(parm.in, wcsstr(argv[i], L":") + 1);
			inFlag = true;
		}
		if (wcsstr(argv[i], PARM_OUT))
		{
			wcscpy_s(parm.out, wcsstr(argv[i], L":") + 1);
			outFlag = true;
		}
		if (wcsstr(argv[i], PARM_LOG))
		{
			wcscpy_s(parm.log, wcsstr(argv[i], L":") + 1);
			logFlag = true;
		}
	}

	if (!inFlag)
	{
		throw ERROR_THROW(100);
	}
	if (!outFlag)
	{
		wcscpy_s(parm.out, parm.in);
		wcscat_s(parm.out, PARM_OUT_DEFAULT_EXT);
	}
	if (!logFlag)
	{
		wcscpy_s(parm.log, parm.in);
		wcscat_s(parm.log, PARM_LOG_DEFAULT_EXT);
	}

	return parm;
}
