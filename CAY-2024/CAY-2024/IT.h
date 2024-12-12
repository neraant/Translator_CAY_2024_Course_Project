#pragma once
#define ID_MAXSIZE					16
#define TI_MAXSIZE					4096
#define TI_STR_DEFAULT				0x00
#define TI_INT_DEFAULT				0
#define TI_BOOL_DEFAULT				false
#define TI_NULLIDX					0xffffffff
#define TI_STR_MAXSIZE				250
#define TI_INT_MINSIZE				-32768
#define TI_INT_MAXSIZE				32767
#define TI_INT_LITERAL_MAXSIZE		(int)0xFF800000
#define TI_INT_LITERAL_MINSIZE		(int)0x007FFFFF
#define TI_FILENAME					"C:\\Users\\chere\\OneDrive\\Рабочий стол\\КП\\CAY-2024\\Debug\\IT.txt"


namespace IT
{
	enum IDDATATYPE { VOID = 0, INT = 1, STR = 2, BOOL = 3, FLOAT = 5 };

	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, W = 5, I = 6, E = 7 };

	struct Entry
	{
		int idxfirstLE;
		char id[ID_MAXSIZE];
		char scope[ID_MAXSIZE];
		IDDATATYPE iddatatype;
		IDTYPE idtype;

		union
		{
			bool vbool;
			int vint;
			float vfloat;
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE - 1];
			} vstr;
		} value;

		Entry();
	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size);
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, char id[ID_MAXSIZE]);
	int Search(IdTable& idtable, Entry entry);
	void WriteTable(IdTable& idtable);
	void Delete(IdTable& idtable);
}