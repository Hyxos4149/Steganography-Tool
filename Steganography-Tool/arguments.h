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

    bool Parse(int argc, char** argv);
    void ShowHelp() const;
    bool IsValid() const;

    std::string GetCommand() const { return command; }
    std::string GetInputFile() const { return inputFile; }
    std::string GetOutputFile() const { return outputFile; }
    std::string GetMessage() const { return message; }
    std::string GetKey() const { return key; }
};