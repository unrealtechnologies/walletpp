//
// Created by ariel on 3/9/24.
//

#ifndef CRYPTO_ALGORITHMS_H
#define CRYPTO_ALGORITHMS_H

#include <botan/hash.h>
#include <string>
#include <vector>

struct crypto_algorithms {
    constexpr static std::string_view pbkdf2_algorithm = "PBKDF2(SHA-512)";
    constexpr static uint16_t pbkdf2_iterations = 2048;
    constexpr static uint16_t sha256_output_byte_size = 32;
    constexpr static uint16_t pbkdf2_sha512_output_byte_size = 64;

    [[nodiscard]] static std::array<uint8_t, sha256_output_byte_size> sha256(const Botan::secure_vector<uint8_t> &contents);
    [[nodiscard]] static std::string to_hex(const Botan::secure_vector<uint8_t> &contents);
    [[nodiscard]] static Botan::secure_vector<uint8_t> from_hex(const std::string &hex_string);
    [[nodiscard]] static std::array<uint8_t, pbkdf2_sha512_output_byte_size> pbkdf2(const std::string_view &password, const std::string &salt);
};


#endif//CRYPTO_ALGORITHMS_H
