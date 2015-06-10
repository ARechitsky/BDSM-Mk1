#pragma once

class LineParser
{
public:
	LineParser();
	~LineParser();

	int parse(char *line);
	char *getcommand();
	char gettype();
	bool getpointerflag();
	int getargument();

private:
	void clean();

	char *m_command;
	char m_type;
	bool m_pointerflag;
	int m_argument;
};

