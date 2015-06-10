#pragma once

typedef unsigned char uchar;
typedef unsigned short int ushort;
typedef unsigned int uint;
class VM;
typedef void(VM::*action)(void);

class VM
{
public:
	VM();
	~VM();

	int executeFile(const char *filename);
	int execute(ushort linecount, const char *byte_code);
private:
	void init();
	void init_page(uchar page);
	int execute_one();

	void com_add();
	void com_assign();
	void com_div();
	void com_goto();
	void com_load();
	void com_mod();
	void com_mul();
	void com_nop();
	void com_page();
	void com_read();
	void com_save();
	void com_sub();
	void com_write();
	
	const char *m_code;
	ushort m_code_size;
	uchar *m_memory_map[256];
	uchar m_page;
	uint m_reg;
	ushort m_ip;
	uint m_regmask;
	uint m_arg;
	action *commands;
	char m_type;
};

