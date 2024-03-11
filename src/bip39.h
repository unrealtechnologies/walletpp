//
// Created by ariel on 3/9/24.
//

#ifndef BIP39_H
#define BIP39_H

#include "crypto_algorithms.h"
#include <climits>
#include <string_view>
#include <vector>

class bip39 {
    constexpr static uint8_t entropy_bits_multiple = 32;
    constexpr static uint8_t entropy_bits_per_word = 11;
    static constexpr int entropy_min_length_in_bytes = 16;
    static constexpr int entropy_max_length_in_bytes = 32;
    constexpr static uint8_t single_byte_size = 1U;
    constexpr static uint8_t single_byte_bits_length = CHAR_BIT;

public:
    [[nodiscard]] static Botan::secure_vector<uint8_t>
    checksum_from_entropy(const Botan::secure_vector<uint8_t> &entropy);

    [[nodiscard]] static Botan::secure_vector<bool>
    checksum_bits_from_entropy(const Botan::secure_vector<uint8_t> &entropy);

    [[nodiscard]] static Botan::secure_vector<uint16_t>
    words_index_from_entropy(const Botan::secure_vector<bool> &entropy_with_checksum);

    [[nodiscard]] static std::vector<std::string_view>
    mnemonic_from_entropy(const Botan::secure_vector<uint8_t> &entropy);

    [[nodiscard]] static std::array<uint8_t, crypto_algorithms::pbkdf2_sha512_output_byte_size>
    seed_from_mnemonic(const std::vector<std::string_view> &words_vector);
};


#endif//BIP39_H
