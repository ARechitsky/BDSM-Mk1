#include "LineParser.h"

#include <stdlib.h>
#include <string.h>

bool isAlpha(char c)
{
	if((c>='a' && c<='z') || (c>='A' && c<='Z'))
		return true;
	return false;
}

LineParser::LineParser()
{
	m_command = 0;
	m_type = 'B';
	m_pointerflag = false;
	m_argument = 0;
}

LineParser::~LineParser()
{
}

int LineParser::parse(char *line)
{
	clean();
	while (*line == ' ' || *line == '\t') line++;
	if (*line == '\'')
	{
		m_command = "";
		m_pointerflag = false;
		m_type = 'B';
		m_argument = 0;
		return 0;
	}

	int nc=0;
	char *p = line;
	while (isAlpha(*line))
	{
		line++;
		nc++;
	}
	m_command = new char[nc+1];
	for (int i = 0; i<nc; i++) 
	{
		m_command[i] = p[i];
	}
	m_command[nc] = 0;

	if (m_command == "") return -1;

	while (*line == ' ' || *line == '\t') line++;

	if (*line == '&')
	{
		m_pointerflag = true;
		line++;
	}

	while (*line == ' ' || *line == '\t') line++;

	switch (*line)
	{
	case 'c':
	case 'C':
		m_type = 'C';
		line++;
		break;
	case 'b':
	case 'B':
		m_type = 'B';
		line++;
		break;
	case 'w':
	case 'W':
		m_type = 'W';
		line++;
		break;
	case 'd':
	case 'D':
		m_type = 'D';
		line++;
		break;
	}

	while (*line == ' ' || *line == '\t') line++;

	m_argument = atoi(line);

	return 0;
}

void LineParser::clean()
{
	if(m_command)
	{
		free(m_command);
	}
	m_command = 0;
	m_pointerflag = false;
	m_type = 'B';
	m_argument = 0;
}

char *LineParser::getcommand()
{
	if (!m_command) 
		return "";
	return m_command;
}

bool LineParser::getpointerflag()
{
	return m_pointerflag;
}

char LineParser::gettype()
{
	return m_type;
}

int LineParser::getargument()
{
	return m_argument;
}