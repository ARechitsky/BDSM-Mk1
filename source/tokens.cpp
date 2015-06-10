#include "BDSCompiler.h"
#include "tokens.h"

#include <string.h>

typedef struct {
	int id;
	const char *name;
	bool has_type;
	bool can_char;
	bool can_pointer;
	bool has_argument;
} tokenType;

static tokenType tokenlist[TOK__LAST] = {
	{TOK_ADD, "ADD", true, false, true, true},
	{TOK_ASSIGN, "ASSIGN", true, false, true, true},
	{TOK_DIV, "DIV", true, false, true, true},
	{TOK_GOTO, "GOTO", true, false, false, true},
	{TOK_LOAD, "LOAD", true, false, false, false},
	{TOK_MOD, "MOD", true, false, true, true},
	{TOK_MUL, "MUL", true, false, true, true},
	{TOK_NOP, "NOP", false, false, false, false},
	{TOK_PAGE, "PAGE", false, false, false, true},
	{TOK_READ, "READ", true, true, false, false},
	{TOK_SAVE, "SAVE", true, false, true, true},
	{TOK_SUB, "SUB", true, false, true, true},
	{TOK_WRITE, "WRITE", true, true, false, false}
};

bool BDSCompiler::is_valid_token(char *s)
{
	for (int x = 0; x < TOK__LAST; x ++)
	{
		if (!stricmp(tokenlist[x].name,s))
		{
			return true;
		}
	}
	return false;
}

int BDSCompiler::get_commandtoken(char *s, bool *ht, bool *cc, bool *cp, bool *ha)
{
	int x;
	for (x = 0; x < TOK__LAST; x ++)
	{
		if (!stricmp(tokenlist[x].name,s)) 
		{
			*ht=tokenlist[x].has_type;
			*cc=tokenlist[x].can_char;
			*cp=tokenlist[x].can_pointer;
			*ha=tokenlist[x].has_argument;
			return tokenlist[x].id;
		}
	}
	return -1;
}
