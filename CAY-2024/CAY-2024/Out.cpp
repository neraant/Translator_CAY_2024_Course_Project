#include "stdafx.h"

Out::OUT Out::GetOut(wchar_t outfile[])
{
	Out::OUT out = INITOUT;
	out.stream = new std::ofstream(outfile);

	if (!(*out.stream).is_open())
	{
		throw ERROR_THROW(112);
	}

	std::wcscpy(out.outfile, outfile);

	return out;
}

void Out::WriteOut(OUT out, In::IN in)
{
	(*out.stream) << in.text;
}

void Out::WriteError(OUT out, Error::ERROR error)
{
	if (error.id != 100)
	{
		(*out.stream) << "������ " << error.id << ": " << error.message
			<< ", ������ " << error.intext.line << ", ������� " << error.intext.col << "\n";
	}
}

void Out::Close(OUT out)
{
	(*out.stream).close();
	delete out.stream;
}
