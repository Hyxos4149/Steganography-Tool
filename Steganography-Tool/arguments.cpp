#include "arguments.h"
#include <iostream>

Arguments::Arguments() {
    command = "";
    inputFile = "";
    outputFile = "";
    message = "";
    key = "";
}



void Arguments::ShowHelp() const {
    std::cout << "Commandes disponibles :" << std::endl;

    std::cout << "encode : cacher un message" << std::endl;
    std::cout << "decode : extraire un message" << std::endl;
    std::cout << "--help : afficher l'aide" << std::endl;
}
