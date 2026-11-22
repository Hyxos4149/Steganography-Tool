#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class BMP {
public:
    // Charge un BMP accepte par le projet.
    static BMP load(const std::string& path);

    // Enregistre l'image sans ecraser un fichier existant.
    void save(const std::string& path) const;

    std::uint32_t width() const noexcept {
        return width_;
    }

    std::uint32_t height() const noexcept {
        return height_;
    }

    std::size_t componentCount() const noexcept;

    // Acces aux composantes couleur B, G et R.
    // Le padding des lignes est ignore.
    std::uint8_t component(std::size_t index) const;
    void setComponent(std::size_t index, std::uint8_t value);

private:
    std::vector<std::uint8_t> bytes_;

    std::uint32_t width_ = 0;
    std::uint32_t height_ = 0;

    std::size_t pixelOffset_ = 0;
    std::size_t rowStride_ = 0;
    std::size_t pixelBytes_ = 0;

    std::size_t componentFileOffset(std::size_t index) const;
};