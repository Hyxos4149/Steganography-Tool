#pragma once

#include <cstdint>
#include <string>
#include <vector>

class BMP {
public:
    // Lit un BMP 24 bits non compressé. Retourne false en cas d'erreur.
    bool read(const std::string& path);

    // Écrit le BMP (en-tête + pixels). Retourne false en cas d'erreur.
    bool write(const std::string& path) const;

    // Accès aux octets de pixels (B, G, R, B, G, R...)
    std::vector<std::uint8_t>& pixels() { return data; }
    const std::vector<std::uint8_t>& pixels() const { return data; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Nombre d'octets utiles pour cacher un message (1 bit par octet)
    std::size_t capacityBits() const { return data.size(); }

private:
    std::vector<std::uint8_t> header; 
    std::vector<std::uint8_t> data;   
    int width = 0;
    int height = 0;
};