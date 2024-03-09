//
// Created by ariel on 3/9/24.
//

#ifndef BIP39_H
#define BIP39_H

#include <string>
#include <vector>

class bip39 {
    constexpr static uint8_t entropy_bits_multiple = 32;
    constexpr static uint8_t entropy_bits_per_word = 11;
    constexpr static uint8_t single_byte_size = 1U;

public:
    [[nodiscard]] static std::vector<uint8_t>
    checksum_from_entropy(const std::vector<uint8_t> &entropy);
    [[nodiscard]] static std::vector<std::string>
    mnemonic_from_entropy(const std::vector<uint8_t> &entropy);
};


#endif//BIP39_H
