#include "stdafx.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log = Log::INITLOG;

		log.stream = new std::ofstream(logfile);

		if (!(*log.stream).is_open())
		{
			throw ERROR_THROW(112);
		}

		return log;
	};


	void WriteLine(LOG log, char* c, ...)
	{
		char** pc = &c;

		while (*pc != "")
		{
			(*log.stream) << *pc;
			pc += 1;
		}

		(*log.stream) << std::endl;
	};


	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t** pw_t = &c;
		char buffer[64];

		while (*pw_t != L"")
		{
			wcstombs(buffer, *pw_t, sizeof(buffer));
			(*log.stream) << buffer;
			pw_t += 1;
		}

		(*log.stream) << std::endl;
	};


	void WriteLog(LOG log)
	{
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		char buf[20];
		strftime(buf, 20, "%Y.%m.%d %H:%M:%S", timeinfo);
		(*log.stream) << "\n---- �������� ---- " << buf << " ----\n";
	};


	void WriteParm(LOG log, Parm::PARM parm)
	{
		char inInfo[PARM_MAX_SIZE];
		char outInfo[PARM_MAX_SIZE];
		char logInfo[PARM_MAX_SIZE];

		wcstombs(inInfo, parm.in, sizeof(inInfo));
		wcstombs(outInfo, parm.out, sizeof(outInfo));
		wcstombs(logInfo, parm.log, sizeof(logInfo));

		(*log.stream) << "--�- ��������� ��--\n"
			<< " -in:	" << inInfo << "\n"
			<< " -out:	" << outInfo << "\n"
			<< " -log:	" << logInfo << "\n";
	};


	void WriteIn(LOG log, In::IN in)
	{
		(*log.stream) << "---- �������� ������ ----\n"
			<< "���������� ��������: " << in.size << "\n"
			<< "���������������: " << in.ignore << "\n"
			<< "���������� �����: " << in.lines << "\n";
	};


	void WriteError(LOG log, Error::ERROR error)
	{
		if (error.id == 100)
		{
			(*log.stream) << "������ " << error.id << ": " << error.message << "\n";
		}
		else
		{
			(*log.stream) << "������ " << error.id << ": " << error.message << " "
				<< "������ " << error.intext.line << ", �������: " << error.intext.col << "\n";
		}
	};


	void Close(LOG log)
	{
		(*log.stream).close();
		delete log.stream;
		log.stream = nullptr;
	}
};