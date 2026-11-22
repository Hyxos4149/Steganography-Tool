#include "bmp.h"

#include <cstdio>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace {
    FILE* openFile(const std::string& path, const char* mode) {
#ifdef _MSC_VER
        FILE* file = nullptr;
        if (fopen_s(&file, path.c_str(), mode) != 0) {
            return nullptr;
        }
        return file;
#else
        return std::fopen(path.c_str(), mode);
#endif
    }

    std::uint16_t read16(const std::uint8_t* p) {
        return static_cast<std::uint16_t>(p[0])
            | static_cast<std::uint16_t>(
                static_cast<std::uint16_t>(p[1]) << 8
                );
    }

    std::uint32_t read32(const std::uint8_t* p) {
        return static_cast<std::uint32_t>(p[0])
            | (static_cast<std::uint32_t>(p[1]) << 8)
            | (static_cast<std::uint32_t>(p[2]) << 16)
            | (static_cast<std::uint32_t>(p[3]) << 24);
    }

    void writeAll(
        FILE* file,
        const std::vector<std::uint8_t>& bytes,
        const std::string& path
    ) {
        if (!bytes.empty()
            && std::fwrite(bytes.data(), 1, bytes.size(), file) != bytes.size()) {
            throw std::runtime_error("echec d'ecriture BMP : " + path);
        }
    }
}

BMP BMP::load(const std::string& path) {
    FILE* file = openFile(path, "rb");

    if (!file) {
        throw std::runtime_error(
            "Impossible d'ouvrir le BMP : " + path
        );
    }

    BMP result;

    try {
        if (std::fseek(file, 0, SEEK_END) != 0) {
            throw std::runtime_error("echec de recherche dans le BMP.");
        }

        const long end = std::ftell(file);

        if (end < 54) {
            throw std::runtime_error("BMP tronque ou trop court.");
        }

        if (std::fseek(file, 0, SEEK_SET) != 0) {
            throw std::runtime_error("echec de recherche dans le BMP.");
        }

        result.bytes_.resize(static_cast<std::size_t>(end));

        if (std::fread(
            result.bytes_.data(),
            1,
            result.bytes_.size(),
            file
        ) != result.bytes_.size()) {
            throw std::runtime_error("Lecture BMP incomplete.");
        }

        if (std::fclose(file) != 0) {
            file = nullptr;
            throw std::runtime_error("echec de fermeture du BMP.");
        }

        file = nullptr;
    }
    catch (...) {
        if (file) {
            std::fclose(file);
        }

        throw;
    }

    const auto* bytes = result.bytes_.data();

    if (bytes[0] != 'B' || bytes[1] != 'M') {
        throw std::runtime_error("Signature BMP invalide.");
    }

    const std::uint32_t declaredSize = read32(bytes + 2);
    result.pixelOffset_ = read32(bytes + 10);

    if (read32(bytes + 14) != 40) {
        throw std::runtime_error(
            "Seul un en-tête BMP de 40 octets est accepte."
        );
    }

    const std::uint32_t rawWidth = read32(bytes + 18);
    const std::uint32_t rawHeight = read32(bytes + 22);

    if (rawWidth == 0
        || rawHeight == 0
        || rawWidth > 0x7FFFFFFFu
        || rawHeight > 0x7FFFFFFFu) {
        throw std::runtime_error(
            "La largeur et la hauteur doivent être positives."
        );
    }

    if (read16(bytes + 26) != 1
        || read16(bytes + 28) != 24
        || read32(bytes + 30) != 0) {
        throw std::runtime_error(
            "Il faut un BMP 24 bits non compresse."
        );
    }

    if (result.pixelOffset_ < 54
        || result.pixelOffset_ > result.bytes_.size()) {
        throw std::runtime_error("Offset des pixels invalide.");
    }

    result.width_ = rawWidth;
    result.height_ = rawHeight;

    const std::uint64_t rawRow =
        static_cast<std::uint64_t>(result.width_) * 3u;

    const std::uint64_t stride =
        (rawRow + 3u) & ~std::uint64_t(3u);

    const std::uint64_t pixelBytes =
        stride * result.height_;

    if (stride > std::numeric_limits<std::size_t>::max()
        || pixelBytes > std::numeric_limits<std::size_t>::max()
        || pixelBytes > result.bytes_.size() - result.pixelOffset_) {
        throw std::runtime_error(
            "Les donnees de pixels sont tronquees ou trop grandes."
        );
    }

    result.rowStride_ = static_cast<std::size_t>(stride);
    result.pixelBytes_ = static_cast<std::size_t>(pixelBytes);

    const std::uint64_t pixelEnd =
        static_cast<std::uint64_t>(result.pixelOffset_) + pixelBytes;

    if (declaredSize != 0
        && (declaredSize > result.bytes_.size()
            || declaredSize < pixelEnd)) {
        throw std::runtime_error(
            "La taille declaree du BMP est incoherente."
        );
    }

    return result;
}

void BMP::save(const std::string& path) const {
    // Le mode "x" evite d'ecraser un fichier dejà present.
    FILE* file = openFile(path, "wbx");

    if (!file) {
        throw std::runtime_error(
            "Impossible de creer le fichier sans en ecraser un autre : "
            + path
        );
    }

    try {
        writeAll(file, bytes_, path);

        if (std::fflush(file) != 0) {
            throw std::runtime_error("echec d'ecriture du BMP.");
        }

        if (std::fclose(file) != 0) {
            file = nullptr;
            throw std::runtime_error("echec de fermeture du BMP.");
        }

        file = nullptr;
    }
    catch (...) {
        if (file) {
            std::fclose(file);
        }

        std::remove(path.c_str());
        throw;
    }
}

std::size_t BMP::componentCount() const noexcept {
    return static_cast<std::size_t>(width_)
        * static_cast<std::size_t>(height_)
        * 3u;
}

std::size_t BMP::componentFileOffset(std::size_t index) const {
    if (index >= componentCount()) {
        throw std::out_of_range("Index de composante hors limites.");
    }

    const std::size_t componentsPerRow =
        static_cast<std::size_t>(width_) * 3u;

    const std::size_t row = index / componentsPerRow;
    const std::size_t column = index % componentsPerRow;

    return pixelOffset_ + row * rowStride_ + column;
}

std::uint8_t BMP::component(std::size_t index) const {
    return bytes_[componentFileOffset(index)];
}

void BMP::setComponent(std::size_t index, std::uint8_t value) {
    bytes_[componentFileOffset(index)] = value;
}