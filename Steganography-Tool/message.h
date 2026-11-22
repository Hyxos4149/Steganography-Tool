#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Message {
public:
    Message() = default;
    explicit Message(const std::string& text);

    // Texte -> bits (avec terminateur '\0' à la fin)
    std::vector<std::uint8_t> toBits() const;

    // Bits -> texte (s'arrête au premier '\0')
    static std::string fromBits(const std::vector<std::uint8_t>& bits);

    const std::string& getText() const { return text; }

private:
    std::string text;
};
