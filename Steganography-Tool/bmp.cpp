#include "bmp.h"

#include <fstream>
#include <iostream>

static std::uint32_t readU32(const std::vector<std::uint8_t>& h, std::size_t pos) {
    return  static_cast<std::uint32_t>(h[pos])
        | static_cast<std::uint32_t>(h[pos + 1]) << 8
        | static_cast<std::uint32_t>(h[pos + 2]) << 16
        | static_cast<std::uint32_t>(h[pos + 3]) << 24;
}

bool BMP::read(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir " << path << "\n";
        return false;
    }

    // 1) Lire les 14 premiers octets (en-tête de fichier)
    std::vector<std::uint8_t> fileHeader(14);
    in.read(reinterpret_cast<char*>(fileHeader.data()), 14);
    if (in.gcount() != 14) {
        std::cerr << "Erreur : en-tête incomplet\n";
        return false;
    }

    // 2) Signature "BM"
    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
        std::cerr << "Erreur : ce n'est pas un fichier BMP\n";
        return false;
    }

    // 3) Offset où commencent les pixels
    std::uint32_t pixelOffset = readU32(fileHeader, 10);
    if (pixelOffset < 14) {
        std::cerr << "Erreur : offset invalide\n";
        return false;
    }

    // 4) Lire le reste de l'en-tête (jusqu'aux pixels)
    std::vector<std::uint8_t> rest(pixelOffset - 14);
    in.read(reinterpret_cast<char*>(rest.data()), static_cast<std::streamsize>(rest.size()));
    if (static_cast<std::size_t>(in.gcount()) != rest.size()) {
        std::cerr << "Erreur : en-tête tronqué\n";
        return false;
    }

    header = fileHeader;
    header.insert(header.end(), rest.begin(), rest.end());

    // 5) Largeur / hauteur / bits par pixel (dans le DIB header, à partir de l'octet 14)
    width = static_cast<int>(readU32(header, 18));
    height = static_cast<int>(readU32(header, 22));
    std::uint16_t bpp = static_cast<std::uint16_t>(header[28] | (header[29] << 8));
    std::uint32_t compression = readU32(header, 30);

    if (bpp != 24 || compression != 0) {
        std::cerr << "Erreur : seuls les BMP 24 bits non compressés sont gérés\n";
        return false;
    }

    // 6) Lire tous les pixels (padding inclus)
    std::size_t rowSize = ((static_cast<std::size_t>(width) * 3 + 3) / 4) * 4;
    std::size_t absHeight = static_cast<std::size_t>(height < 0 ? -height : height);
    std::size_t dataSize = rowSize * absHeight;

    data.assign(dataSize, 0);
    in.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(dataSize));
    if (static_cast<std::size_t>(in.gcount()) != dataSize) {
        std::cerr << "Erreur : données de pixels incomplètes\n";
        return false;
    }

    return true;
}

bool BMP::write(const std::string& path) const {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Erreur : impossible de créer " << path << "\n";
        return false;
    }

    out.write(reinterpret_cast<const char*>(header.data()), static_cast<std::streamsize>(header.size()));
    out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));

    return out.good();
}