#include "BDSCompiler.h"
#include "LineParser.h"
#include "byte_code.h"

#include <stdio.h>
#include <string.h>

BDSCompiler::BDSCompiler(void)
{
	fp = 0;
	linecnt = 0;
	byte_code.resize(0);
}


BDSCompiler::~BDSCompiler(void)
{
}

int BDSCompiler::parseScript()
{
	char str[MAX_LINELENGTH];

	for (;;)
	{
        char *p=str;
		*p=0;
		fgets(str,MAX_LINELENGTH,fp);
		if (feof(fp)&&!str[0]) break;

		// remove trailing whitespace
		while (*p) p++;
		if (p > str) p--;
		while (p >= str && (*p == '\r' || *p == '\n' || *p == ' ' || *p == '\t')) p--;
		*++p=0;

		int ret=doParse(str);

		if (ret != PS_OK) return ret;
	}

	return PS_OK;
}

int BDSCompiler::doParse(const char *str)
{
	LineParser line;
	int res = line.parse((char *)str);
	if (res)
	{
		return PS_ERROR;
	}

	if(!stricmp(line.getcommand(),""))
	{
		return PS_OK;
	}

	bool ht, cc, cp, ha;
	int tkid = get_commandtoken(line.getcommand(),&ht,&cc,&cp,&ha);
	if (tkid == -1)
	{
		return PS_ERROR;
	}

	char type = line.gettype();
	bool pointermode = line.getpointerflag();
	int arg = line.getargument();

	if (!ht && type != 'B')
	{
		return PS_ERROR;
	}

	if (!cc && type == 'C')
	{
		return PS_ERROR;
	}

	if (!cp && pointermode)
	{
		return PS_ERROR;
	}

	if (!ha && arg)
	{
		return PS_ERROR;
	}

	char command_code[5];
	for (int i=0; i<5; i++)
	{
		command_code[i] = 0;
	}
	command_code[0] = tkid << 4;
	command_code[0] |= RESERVED;
	switch (type)
	{
	case 'C':
		command_code[0] |= TYPE_CHAR;
		break;
	case 'W':
		command_code[0] |= TYPE_WORD;
		break;
	case 'D':
		command_code[0] |= TYPE_DWORD;
		break;
	default:
		command_code[0] |= TYPE_BYTE;
		break;
	}
	command_code[0] |= pointermode ? POINTERMODE_ON : POINTERMODE_OFF;
	*(int *)(command_code + 1) = arg;

	byte_code.add(command_code, 5);
	linecnt++;

	return PS_OK;
}

int BDSCompiler::getLineCount()
{
	return linecnt;
}

const char *BDSCompiler::getByteCode()
{
	return (const char *)byte_code.get();
}

int BDSCompiler::Open(const char *filename)
{
	fp = fopen(filename, "r");
	if (!fp)
	{
		return PS_FILE_ERROR;
	}
	int res = parseScript();
	fclose(fp);
	return res;
}

int BDSCompiler::Save(const char *filename)
{
	FILE *f = fopen(filename, "wb");
	if (!fp)
	{
		return PS_FILE_ERROR;
	}
	fwrite(&linecnt, sizeof(linecnt), 1, f);
	fwrite(getByteCode(), 5, linecnt, f);
	fclose(f);
	return 0;
}
