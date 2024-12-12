#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Ru");
	Parm::PARM parm;
	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INITOUT;
	try
	{
		parm = Parm::getparm(argc, argv);

		log = Log::getlog(parm.log);
		out = Out::GetOut(parm.out);
		In::IN in = In::getin(parm.in);

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		LT::LexTable lexTable;
		IT::IdTable idTable;

		LA::LEX lex = LA::LA(parm, in);
		cout << "Лексический анализ выполнен успешно!" << endl;
		
		MFST_TRACE_START(*log.stream);
		MFST::Mfst mfst(lex.lextable, GRB::getGreibach());
		mfst.start(log);
		mfst.savededucation();
		mfst.printrules(log);

		SA::SA(lex.lextable, lex.idtable);
		cout << "Семантический анализ выполнен успешно!" << endl;

		PN::PN(lex.lextable, lex.idtable);
		LT::WriteTable(lex.lextable);

		GEN::AsmGeneration(lex, out);
		cout << "Генерация кода выполнена успешно!" << endl;

		LT::Delete(lex.lextable);
		IT::Delete(lex.idtable);
	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << ':' << e.message << endl;
		if (e.intext.line != -1)
			cout << "Строка:" << e.intext.line;
		if (e.intext.col != -1)
			cout << " Позиция:" << e.intext.col;

		cout << endl;

		Log::WriteError(log, e);
		Out::WriteError(out, e);
	}

	Log::Close(log);
	Out::Close(out);

	return 0;
}