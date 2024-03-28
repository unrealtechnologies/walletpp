//
// Created by ariel on 3/9/24.
//

#ifndef CRYPTO_ALGORITHMS_H
#define CRYPTO_ALGORITHMS_H

#include "constants.h"
#include "secure_vector.h"
#include <botan/hash.h>
#include <string>
#include <vector>


namespace walletpp {
    struct crypto_algorithms {
        [[nodiscard]] static secure_vector<uint8_t> sha256(const secure_vector<uint8_t> &contents);
        [[nodiscard]] static secure_vector<uint8_t> double_sha256(const secure_vector<uint8_t> &contents);
        [[nodiscard]] static secure_vector<uint8_t> ripemd160(const secure_vector<uint8_t> &contents);
        [[nodiscard]] static secure_vector<uint8_t> keccak256(const secure_vector<uint8_t> &contents);
        [[nodiscard]] static auto keccak256(const std::string &contents) -> secure_vector<uint8_t>;
        // [[nodiscard]] static std::string to_hex(const std::span<uint8_t> &contents);
        // [[nodiscard]] static std::string to_hex(const std::span<uint8_t> &contents);
        [[nodiscard]] static std::string to_hex(const secure_vector<uint8_t>& v);
        [[nodiscard]] static secure_vector<uint8_t> from_hex(const std::string &hex_string);
        // [[nodiscard]] static secure_vector<uint8_t> from_hex(const std::string &hex_string);
        [[nodiscard]] static std::array<uint8_t, pbkdf2_sha512_output_byte_size> fast_pbkdf2(const std::string_view password, const std::string_view salt,
                                                                                             const size_t iterations = default_pbkdf2_iterations);
        [[nodiscard]] static secure_vector<uint8_t> binary_from_bytes(const secure_vector<uint8_t> &bytes, const std::optional<size_t> &num_of_bits);
        [[nodiscard]] static secure_vector<uint8_t> hmac512(const secure_vector<uint8_t> &msg, const secure_vector<uint8_t> &key);
        [[nodiscard]] static secure_vector<uint8_t> generate_private_key(const secure_vector<uint8_t> &key, const secure_vector<uint8_t> &tweak);
        [[nodiscard]] static secure_vector<uint8_t> generate_public_key(const secure_vector<uint8_t> &key, bool compressed);
        [[nodiscard]] static secure_vector<uint8_t> generate_entropy(size_t bytes_size);
        [[nodiscard]] static uint32_t htobe32(uint32_t x);
        [[nodiscard]] static secure_vector<uint8_t> uint32_to_big_endian_vector(uint32_t num);
        [[nodiscard]] static secure_vector<uint8_t> uint32_to_big_endian_bytes(uint32_t value);
        [[nodiscard]] static auto base58_encode_to_string(secure_vector<uint8_t> data) -> std::string;
        [[nodiscard]] static auto base58_decode_to_vector(const std::string &base58_string) -> secure_vector<uint8_t>;

        template<std::size_t SIZE>
        static void secure_erase_array(std::array<uint8_t, SIZE> &arr) {
            std::ranges::fill(arr, 0);
        }

        static void secure_erase_vector(std::vector<uint8_t> &vec) { std::ranges::fill(vec, 0); }

        static void secure_erase_string(std::string &str) {
            std::ranges::fill(str, 0);
            str.clear();
            str.shrink_to_fit();
        }
    };
}// namespace walletpp


#endif//CRYPTO_ALGORITHMS_H
