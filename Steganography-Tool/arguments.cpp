#include "arguments.h"
#include <iostream>

//list of arguments: encode, decode, --help
Arguments::Arguments() {
    command = "";
    inputFile = "";
    outputFile = "";
    message = "";
    key = "";
}


// Function to display help information
void Arguments::ShowHelp() const {
    std::cout << "Commandes disponibles :" << std::endl;

    std::cout << "encode : cacher un message" << std::endl;
    std::cout << "decode : extraire un message" << std::endl;
    std::cout << "--help : afficher l'aide" << std::endl;
}
