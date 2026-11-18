#pragma once
#include <string>
class arguments
{
private: 
	std::string m_command;
	std::string m_source;
	std::string m_destination;
	std::string m_message;
	std::string m_key;
public: 
	arguments();
	void argumentParser(int argc, char* argv);
	void validateArguments();
	std::string GetCommand(  );
	std::string GetSource();
	std::string GetDestination();
	std::string GetMessage();
	std::string GetKey();

	void argumentParser(int argc, char* argv);












};

