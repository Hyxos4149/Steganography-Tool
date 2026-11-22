#include "message.h"

#include <cstdint>
#include <limits>
#include <stdexcept>

namespace message {
    namespace {
        constexpr std::uint8_t kMagic[4] = {
            'S', 'T', 'G', '1'
        };

        constexpr std::size_t kHeaderBits =
            Metadata::kSize * 8u;

        void putByte(
            BMP& image,
            std::size_t& bitPosition,
            std::uint8_t byte
        ) {
            for (int bit = 7; bit >= 0; --bit) {
                const auto oldValue = image.component(bitPosition);

                const auto newValue =
                    static_cast<std::uint8_t>(
                        (oldValue & 0xFEu)
                        | ((byte >> bit) & 1u)
                        );

                image.setComponent(bitPosition, newValue);
                ++bitPosition;
            }
        }

        std::uint8_t getByte(
            const BMP& image,
            std::size_t& bitPosition
        ) {
            std::uint8_t value = 0;

            for (int bit = 0; bit < 8; ++bit) {
                value = static_cast<std::uint8_t>(
                    (value << 1)
                    | (image.component(bitPosition++) & 1u)
                    );
            }

            return value;
        }
    }

    std::size_t capacityBytes(const BMP& image) {
        const std::size_t components = image.componentCount();
        const std::size_t totalBytes = components / 8u;

        if (totalBytes < Metadata::kSize) {
            return 0;
        }

        return totalBytes - Metadata::kSize;
    }

    void embed(
        BMP& image,
        const std::string& text,
        const std::string& key
    ) {
        if (text.size() > capacityBytes(image)) {
            throw std::length_error(
                "Le message est trop long pour cette image."
            );
        }

        if constexpr (sizeof(std::size_t) > sizeof(std::uint64_t)) {
            if (text.size()
            > static_cast<std::size_t>(
                std::numeric_limits<std::uint64_t>::max()
                )) {
                throw std::length_error("Le message est trop long.");
            }
        }

        const std::uint64_t length =
            static_cast<std::uint64_t>(text.size());

        std::size_t position = 0;

        // Signature.
        for (const auto byte : kMagic) {
            putByte(image, position, byte);
        }

        // Longueur sur 8 octets, du plus significatif au moins significatif.
        for (unsigned i = 0; i < 8; ++i) {
            const unsigned shift = (7u - i) * 8u;

            putByte(
                image,
                position,
                static_cast<std::uint8_t>((length >> shift) & 0xFFu)
            );
        }

        // Indicateur : 0 = sans XOR, 1 = avec XOR.
        putByte(image, position, key.empty() ? 0 : 1);

        // Message, éventuellement transformé par XOR.
        for (std::size_t i = 0; i < text.size(); ++i) {
            const auto plain =
                static_cast<std::uint8_t>(
                    static_cast<unsigned char>(text[i])
                    );

            const auto keyByte = key.empty()
                ? 0u
                : static_cast<unsigned char>(key[i % key.size()]);

            putByte(
                image,
                position,
                static_cast<std::uint8_t>(plain ^ keyByte)
            );
        }
    }

    std::string extract(
        const BMP& image,
        const std::string& key
    ) {
        if (image.componentCount() < kHeaderBits) {
            throw std::runtime_error(
                "L'image est trop petite pour contenir les métadonnées."
            );
        }

        std::size_t position = 0;

        // Vérifier la signature.
        for (const auto expected : kMagic) {
            if (getByte(image, position) != expected) {
                throw std::runtime_error(
                    "Aucun message valide n'a été trouvé."
                );
            }
        }

        // Lire la longueur.
        std::uint64_t declaredLength = 0;

        for (unsigned i = 0; i < 8; ++i) {
            declaredLength =
                (declaredLength << 8)
                | getByte(image, position);
        }

        // Lire l'indicateur de chiffrement.
        const auto flag = getByte(image, position);

        if (flag > 1) {
            throw std::runtime_error(
                "Indicateur de chiffrement invalide."
            );
        }

        if (declaredLength > capacityBytes(image)) {
            throw std::runtime_error(
                "La longueur du message dépasse la capacité de l'image."
            );
        }

        if ((flag == 1) != !key.empty()) {
            throw std::runtime_error(
                "La clé fournie ne correspond pas au message."
            );
        }

        const auto length =
            static_cast<std::size_t>(declaredLength);

        std::string result(length, '\0');

        for (std::size_t i = 0; i < length; ++i) {
            const auto encoded = getByte(image, position);

            const auto keyByte = flag
                ? static_cast<unsigned char>(key[i % key.size()])
                : 0u;

            result[i] = static_cast<char>(encoded ^ keyByte);
        }

        return result;
    }
}