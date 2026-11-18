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
	void argumentParser(int argc, char** argv);
	void validateArguments(){	argumentParser(5, nullptr);	}

	std::string GetCommand(){	return m_command;}
	std::string GetSource(){	return m_source;}
	std::string GetDestination(){	return m_destination;}
	std::string GetMessage(){	return m_message;}
	std::string GetKey(){	return m_key;}













};

