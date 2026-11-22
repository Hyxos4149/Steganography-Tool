#pragma once

#include "bmp.h"

#include <cstddef>
#include <string>

namespace message {
    struct Metadata {
        static constexpr std::size_t kSize = 13;
    };

    // Cache le texte dans l'image.
    // Une clé vide signifie qu'il n'y a pas de XOR.
    void embed(
        BMP& image,
        const std::string& text,
        const std::string& key = ""
    );

    // Extrait le texte de l'image.
    std::string extract(
        const BMP& image,
        const std::string& key = ""
    );

    // Renvoie le nombre maximal d'octets de texte cachables.
    std::size_t capacityBytes(const BMP& image);
}