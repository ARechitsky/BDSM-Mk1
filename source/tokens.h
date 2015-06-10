#pragma once

enum
{
	//Register/memory
	TOK_ASSIGN,
	TOK_SAVE,
	TOK_LOAD,
	TOK_PAGE,

	//Input/output
	TOK_READ,
	TOK_WRITE,

	//Arithmetic
	TOK_ADD,
	TOK_SUB,
	TOK_MUL,
	TOK_DIV,
	TOK_MOD,

	//Management
	TOK_GOTO,
	TOK_NOP,

	TOK__LAST
};