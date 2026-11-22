#include "arguments.h"

#include <iostream>

Arguments::Arguments() : command(), inputFile(), outputFile(), message(), key()
{}

bool Arguments::Parse(int argc, char** argv)
{
    if (argc < 2) {
        return false;
    }

    command = argv[1];

    if (command == "--help") {
        return true;
    }

    if (command != "encode" && command != "decode") {
        return false;
    }

    for (int i = 2; i < argc; ++i) {
        const std::string option = argv[i];

        if (i + 1 >= argc) {
            return false;
        }

        const std::string value = argv[++i];

        if (option == "-i" || option == "--input") {
            inputFile = value;
        }
        else if (option == "-o" || option == "--output") {
            outputFile = value;
        }
        else if (option == "-m" || option == "--message") {
            message = value;
        }
        else if (option == "-k" || option == "--key") {
            key = value;
        }
        else {
            return false;
        }
    }

    return true;
}

bool Arguments::IsValid() const
{
    if (command == "--help") {
        return true;
    }

    if (command == "encode") {
        return !inputFile.empty()
            && !outputFile.empty()
            && !message.empty();
    }

    if (command == "decode") {
        return !inputFile.empty();
    }

    return false;
}

void Arguments::ShowHelp() const
{
    std::cout
        << "Utilisation :\n"
        << "  Steganography-Tool.exe encode -i image.bmp -o sortie.bmp "
        "-m \"message\"\n"
        << "  Steganography-Tool.exe decode -i image.bmp\n"
        << "  Steganography-Tool.exe --help\n\n"
        << "Options :\n"
        << "  -i, --input    Image BMP d'entree\n"
        << "  -o, --output   Image BMP de sortie (encode)\n"
        << "  -m, --message  Message à cacher (encode)\n"
        << "  -k, --key      Cle facultative\n";
}