#include "bmp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <cstdint>

int lecture_bmp()
{
    std::ifstream file("image.bmp", std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file");
    }
    else if (file.is_open()) {
        std::cout << "File opened successfully" << std::endl;

    }

    constexpr std::size_t buffer_size = 500;
    char buffer[buffer_size];

    file.read(buffer, buffer_size);

    std::streamsize bytes_read = file.gcount();

    std::string result(
        buffer,
        static_cast<std::size_t>(bytes_read)
    );

    char signature[2] = { result[0], result[1] };

    std::cout << "Signature : "
        << signature[0] << signature[1] << '\n';

    if (signature[0] == 'B' && signature[1] == 'M')
    {
        std::cout << "La signature BMP est reconnue.\n";
    }
    else
    {
        std::cout << "Ce fichier ne semble pas etre un BMP.\n";
    }
}