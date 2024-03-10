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
    [[nodiscard]] static std::string to_hex(const std::span<uint8_t> &contents);
    [[nodiscard]] static Botan::secure_vector<uint8_t> from_hex(const std::string &hex_string);
    [[nodiscard]] static std::array<uint8_t, pbkdf2_sha512_output_byte_size> pbkdf2(const Botan::secure_vector<uint8_t> &password, const Botan::secure_vector<uint8_t> &salt);
    [[nodiscard]] static std::array<uint8_t, pbkdf2_sha512_output_byte_size> pbkdf2(std::string_view password, std::string_view salt);
    [[nodiscard]] static Botan::secure_vector<bool> binary_from_bytes(const Botan::secure_vector<uint8_t> &bytes, const std::optional<size_t> &num_of_bits);

    template<std::size_t SIZE>
    static void secure_erase_array(std::array<uint8_t, SIZE> &arr) {
        std::ranges::fill(arr, 0);
    }

    static void secure_erase_vector(std::vector<uint8_t> &vec) {
        std::ranges::fill(vec, 0);
    }

    static void secure_erase_string(std::string &str) {
        std::ranges::fill(str, 0);
        str.clear();
        str.shrink_to_fit();
    }
};


#endif//CRYPTO_ALGORITHMS_H
