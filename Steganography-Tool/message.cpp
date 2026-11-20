#include "Message.h"

Message::Message(const std::string& t) : text(t) {}

std::vector<std::uint8_t> Message::toBits() const {
    std::vector<std::uint8_t> bits;

    // On ajoute '\0' pour marquer la fin du message
    std::string full = text;
    full.push_back('\0');

    for (char c : full) {
        std::uint8_t byte = static_cast<std::uint8_t>(c);
        // du bit de poids fort (7) au bit de poids faible (0)
        for (int i = 7; i >= 0; --i) {
            bits.push_back(static_cast<std::uint8_t>((byte >> i) & 1));
        }
    }
    return bits;
}

std::string Message::fromBits(const std::vector<std::uint8_t>& bits) {
    std::string result;

    for (std::size_t i = 0; i + 8 <= bits.size(); i += 8) {
        std::uint8_t byte = 0;
        for (int j = 0; j < 8; ++j) {
            byte = static_cast<std::uint8_t>((byte << 1) | (bits[i + j] & 1));
        }
        if (byte == 0) break; // terminateur trouvé
        result.push_back(static_cast<char>(byte));
    }
    return result;
}
