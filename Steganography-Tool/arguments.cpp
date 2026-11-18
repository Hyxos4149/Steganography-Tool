#include "arguments.h"

arguments::arguments()
{
	m_command = "";
	m_source = "";
	m_destination = "";
	m_message = "";
	m_key = "";
}

void arguments::argumentParser(int argc, char** argv)
{
	m_source = argv[1];
	m_destination = argv[2];
	m_message = argv[3];
	m_key = argv[4];
}
