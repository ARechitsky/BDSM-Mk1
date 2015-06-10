#include "VM.h"
#include "byte_code.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

VM::VM()
{
	action coms[13] = {&VM::com_assign, 
		&VM::com_save,
		&VM::com_load,
		&VM::com_page,
		&VM::com_read,
		&VM::com_write,
		&VM::com_add,
		&VM::com_sub,
		&VM::com_mul,
		&VM::com_div,
		&VM::com_mod,
		&VM::com_goto,
		&VM::com_nop};
	commands = new action [13];
	for (int i=0; i<13; i++)
	{
		commands[i] = coms[i];
	}

	for (int i=0; i<256; i++)
	{
		m_memory_map[i] = 0;
	}
}


VM::~VM()
{
}

void VM::com_add()
{
	m_reg += m_arg;
	m_reg &= m_regmask;
	m_ip++;
}

void VM::com_assign()
{
	m_reg = m_arg;
	m_reg &= m_regmask;
	m_ip++;
}

void VM::com_div()
{
	m_reg &= m_regmask;
	m_arg &= m_regmask;
	m_reg /= m_arg;
	m_ip++;
}

void VM::com_goto()
{
	m_reg &= m_regmask;
	if (m_reg)
		m_ip = m_arg & 0xFFFF;
	else
		m_ip++;
}

void VM::com_load()
{
	m_reg &= 0xFF;
	m_reg = *(uint *)(m_memory_map[m_page] + m_reg);
	m_reg &= m_regmask;
	m_ip++;
}

void VM::com_mod()
{
	m_reg &= m_regmask;
	m_arg &= m_regmask;
	m_reg %= m_arg;
	m_ip++;
}

void VM::com_mul()
{
	m_reg *= m_arg;
	m_reg &= m_regmask;
	m_ip++;
}

void VM::com_nop()
{
	m_ip++;
}

void VM::com_page()
{
	m_page = m_arg & m_regmask;
	if (!m_memory_map[m_page])
	{
		init_page(m_page);
	}
	m_ip++;
}

void VM::com_read()
{
	if (m_type == TYPE_CHAR)
	{
		m_reg = getchar();
	}
	else
	{
		char str[10];
		gets(str);
		m_reg = atoi(str);
	}
	m_reg &= m_regmask;
	m_ip++;
}

void VM::com_save()
{
	m_arg &= 0xFF;
	m_reg &= m_regmask;
	uint tmp = *(uint *)(m_memory_map[m_page] + m_arg);
	tmp &= ~m_regmask;
	tmp |= m_reg;
	*(uint *)(m_memory_map[m_page] + m_arg) = tmp;
	m_ip++;
}

void VM::com_sub()
{
	m_reg -= m_arg;
	m_reg &= m_regmask;
	m_ip++;
}

void VM::com_write()
{
	m_reg &= m_regmask;
	if (m_type == TYPE_CHAR)
	{
		putchar(m_reg);
	}
	else
	{
		printf("%d ", m_reg);
	}
	m_ip++;
}

void VM::init_page(uchar page)
{
	m_memory_map[page] = new uchar [259];
	for (int i=0; i<259; i++)
	{
		m_memory_map[page][i] = 0;
	}
}

void VM::init()
{
	m_arg = 0;
	m_ip = 0;
	m_page = 0;
	m_reg = 0;
	m_regmask = 0;
	m_type = 0;
	for (int i=0; i<256; i++)
	{
		if(m_memory_map[i])
		{
			free(m_memory_map[i]);
		}
		m_memory_map[i] = 0;
	}
	init_page(0);
}

int VM::execute_one()
{
	char line[5];
	memcpy(&line, m_code + m_ip*5, 5);
	int command_code = (line[0] & COMMAND_MASK) >> 4;
	if (command_code > MAX_COMMAND)
	{
		return 50;
	}
	action command = commands[command_code];

	m_arg = *(uint *)(line+1);

	char pointer_mode = line[0] & POINTERMODE_MASK;
	if (pointer_mode == POINTERMODE_ON)
	{
		m_arg &= 0xFF;
		m_arg = *(uint *)(m_memory_map[m_page] + m_arg);
	}

	m_type = line[0] & TYPE_MASK;
	switch (m_type)
	{
	case TYPE_DWORD:
		m_regmask = 0xFFFFFFFF;
		break;
	case TYPE_WORD:
		m_regmask = 0x0000FFFF;
		break;
	default:
		m_regmask = 0x000000FF;
	}

	(this->*command)();
	return 0;
}

int VM::execute(ushort linecount, const char *byte_code)
{
	m_code = byte_code;
	m_code_size = linecount;
	init();
	while (m_ip < m_code_size)
	{
		int res = execute_one();
		if (res) return res;
	}
	return 0;
}

int VM::executeFile(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp) return -1;
	ushort size = 0;
	int tmp;
	while ((tmp = fgetc(fp))!=EOF) size++;
	fflush(fp);
	char *code = new char [size];
	fread(code, sizeof(char), size, fp);
	return execute(size/5, code);
}