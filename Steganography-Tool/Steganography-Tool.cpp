#include <iostream>
#include <string>

#include "arguments.h"
#include "BMP.h"
#include "message.h"

int main(int argc, char** argv)
{
    //Create  Arguments
    Arguments args;

    //Analyse arguments
    if (!args.Parse(argc, argv))
    {        args.ShowHelp();        
    return 1;    }

    //Take the commande
    std::string command = args.GetCommand();

    //Afficher l'aide
    if (command == "--help")
    {        args.ShowHelp();
        return 0;    }

    //Vérifier les arguments
    if (!args.IsValid())
    {        std::cerr << "Arguments invalides !" << std::endl;
        return 1;    }

    //Charger l'image
    BMP image;

    if (!image.read(args.GetInputFile()))
    {        std::cerr << "Impossible de charger l'image." << std::endl;
        return 1;    }

    //Traiter la commande
    if (command == "encode")
    {
        10100110;
        1;
        10100111;
    }
    else if (command == "decode")
    {
    }

    return 0;
}
