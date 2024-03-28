//
// Created by ariel on 3/9/24.
//

#ifndef BIP39_H
#define BIP39_H

#include "crypto_algorithms.h"
#include "secure_vector.h"
#include <climits>
#include <string_view>
#include <vector>

namespace walletpp {
    class bip39 {
        constexpr static uint8_t entropy_bits_multiple = 32;
        constexpr static uint8_t entropy_bits_per_word = 11;
        static constexpr int entropy_min_length_in_bytes = 16;
        static constexpr int entropy_max_length_in_bytes = 32;
        constexpr static uint8_t single_byte_bits_length = CHAR_BIT;

    public:
        [[nodiscard]] static walletpp::secure_vector<uint8_t> checksum_from_entropy(const walletpp::secure_vector<uint8_t> &entropy);

        [[nodiscard]] static secure_vector<uint8_t> checksum_bits_from_entropy(const walletpp::secure_vector<uint8_t> &entropy);

        [[nodiscard]] static Botan::secure_vector<uint16_t> words_index_from_entropy(const Botan::secure_vector<bool> &entropy_with_checksum);

        [[nodiscard]] static std::vector<std::string_view> mnemonic_from_entropy(const walletpp::secure_vector<uint8_t> &entropy);

        [[nodiscard]] static std::array<uint8_t, pbkdf2_sha512_output_byte_size> seed_from_mnemonic(const std::vector<std::string_view> &words_vector,
                                                                                                    std::string_view salt = walletpp::default_mnemonic_salt_string,
                                                                                                    size_t number_of_pbkdf2_iterations = walletpp::default_pbkdf2_iterations);
    };
}// namespace walletpp


#endif//BIP39_H
