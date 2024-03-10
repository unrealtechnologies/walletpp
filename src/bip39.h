//
// Created by ariel on 3/9/24.
//

#ifndef BIP39_H
#define BIP39_H

#include "crypto_algorithms.h"
#include <string>
#include <vector>

class bip39 {
    constexpr static uint8_t entropy_bits_multiple = 32;
    constexpr static uint8_t entropy_bits_per_word = 11;
    static constexpr int entropy_min_length_in_bytes = 16;
    static constexpr int entropy_max_length_in_bytes = 32;
    constexpr static uint8_t single_byte_size = 1U;

public:
    [[nodiscard]] static Botan::secure_vector<uint8_t>
    checksum_from_entropy(const Botan::secure_vector<uint8_t> &entropy);
    [[nodiscard]] static Botan::secure_vector<uint16_t>
    words_index_from_entropy(const Botan::secure_vector<uint8_t> &entropy_with_checksum);
    [[nodiscard]] static std::vector<std::string>
    mnemonic_from_entropy(const Botan::secure_vector<uint8_t> &entropy);
    [[nodiscard]] static std::vector<std::string>
    words_from_words_index(const Botan::secure_vector<uint16_t> &words_index);
};


#endif//BIP39_H
