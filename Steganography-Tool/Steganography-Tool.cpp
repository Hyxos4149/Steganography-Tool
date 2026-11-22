#include <exception>
#include <iostream>
#include <string>

#include "arguments.h"
#include "bmp.h"
#include "message.h"

int main(int argc, char** argv) {
    Arguments args;

    if (!args.Parse(argc, argv)) {
        args.ShowHelp();
        return 1;
    }

    const std::string command = args.GetCommand();

    if (command == "--help") {
        args.ShowHelp();
        return 0;
    }

    if (!args.IsValid()) {
        std::cerr << "Arguments invalides !" << std::endl;
        return 1;
    }

    try {
        BMP image = BMP::load(args.GetInputFile());

        if (command == "encode") {
            message::embed(
                image,
                args.GetMessage(),
                args.GetKey()
            );

            image.save(args.GetOutputFile());

            std::cout << "Message cache avec succes." << std::endl;
        }
        else if (command == "decode") {
            const std::string text =
                message::extract(image, args.GetKey());

            std::cout << text << std::endl;
        }
        else {
            std::cerr << "Commande inconnue : "
                << command << std::endl;
            return 1;
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Erreur : " << error.what() << std::endl;
        return 1;
    }

    return 0;
}