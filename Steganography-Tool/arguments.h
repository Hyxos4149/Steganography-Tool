#include <string>

class Arguments {
private:
    std::string command;
    std::string inputFile;
    std::string outputFile;
    std::string message;
    std::string key;

public:
    Arguments();

	// Function to parse command-line arguments
	bool Parse(int argc, char** argv) { return false; }

    void ShowHelp() const;

	bool IsValid() const { return false; }

	// Getters for the private members
	std::string GetCommand() const {		return command;	}
    std::string GetInputFile() const {        return inputFile;    }
    std::string GetOutputFile() const {        return outputFile;    }
    std::string GetMessage() const {        return message;    }
    std::string GetKey() const {        return key;    }
};
