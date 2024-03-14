//
// Created by ariel on 3/11/24.
//

#ifndef HD_DERIVATION_H
#define HD_DERIVATION_H

#include "extended_key.h"
#include <array>


class hd_derivation {
    constexpr static size_t version_array_length = 4;
    const std::array<uint8_t, version_array_length> privateVersion = {0x04, 0x88, 0xAD, 0xE4};
    const std::array<uint8_t, version_array_length> publicVersion = {0x04, 0x88, 0xB2, 0x1E};


public:
    static bool is_hardened_key(size_t index) {
        return index >= walletpp::hardened_key_start_index;
    }

    static void child_key_derivation(const extended_key &parent_key, const size_t index) {
        auto is_hardered = is_hardened_key(index);

        // lets just start with the private derivation
    }

    static extended_key private_child_key_derivation(const extended_key &parent_key, const size_t index) {
        if (parent_key.key.size() != walletpp::private_key_bytes_length) {
            throw std::runtime_error("The private key should be 256 bits, are you attempting to generate the pair with a public key?");
        }
        const auto is_hardered = is_hardened_key(index);
        const auto parent_public_key = crypto_algorithms::generate_public_key(parent_key.key, true);
        const auto index_be_format = crypto_algorithms::uint32_to_big_endian_bytes(index);

        Botan::secure_vector<uint8_t> output{};
        if (is_hardered) {
            Botan::secure_vector<uint8_t> p_key = {walletpp::zero_byte};
            p_key.insert(p_key.end(), parent_key.key.begin(), parent_key.key.end());
            p_key.insert(p_key.end(), index_be_format.begin(), index_be_format.end());
            output = crypto_algorithms::hmac512(parent_key.chain_code, p_key);
        } else {
            Botan::secure_vector<uint8_t> p_key{};
            p_key.insert(p_key.end(), parent_public_key.begin(), parent_public_key.end());
            p_key.insert(p_key.end(), index_be_format.begin(), index_be_format.end());
            output = crypto_algorithms::hmac512(p_key, parent_key.chain_code);
        }

        const Botan::secure_vector<uint8_t> IL = {output.begin(), output.begin() + walletpp::private_key_bytes_length};
        const Botan::secure_vector<uint8_t> IR = {output.begin() + walletpp::private_key_bytes_length, output.end()};

        //The returned child key ki is parse256(IL) + kpar (mod n).
        const auto private_key = crypto_algorithms::generate_private_key(parent_key.key, IL);
        const auto fingerprint = extended_key::fingerprint(parent_public_key);
        auto extended_private_key = extended_key(
                private_key,
                IR,
                fingerprint,
                parent_key.depth + 1,
                index);

        return extended_private_key;
    }

    static extended_key public_child_key_derivation(const extended_key &parent_key, const size_t index) {
        const auto private_key = private_child_key_derivation(parent_key, index);
        const auto public_key = crypto_algorithms::generate_public_key(private_key.key, true);
        const auto fingerprint = extended_key::fingerprint(public_key);
        auto extended_public_key = extended_key(
                public_key,
                private_key.chain_code,
                fingerprint,
                private_key.depth + 1,
                index);

        return extended_public_key;
    }

    static std::pair<extended_key, extended_key> private_and_public_key_pair_derivation(const extended_key &parent_key, const size_t index) {
        if (parent_key.key.size() != walletpp::private_key_bytes_length) {
            throw std::runtime_error("The private key should be 256 bits, are you attempting to generate the pair with a public key?");
        }
        const auto extended_private_key = private_child_key_derivation(parent_key, index);
        const auto public_key = crypto_algorithms::generate_public_key(extended_private_key.key, true);
        auto extended_public_key = extended_key(
                public_key,
                extended_private_key.chain_code,
                extended_private_key.parent_finger_print,
                extended_private_key.depth,
                extended_private_key.index);

        return std::make_pair(extended_private_key, extended_public_key);
    }
};


#endif//HD_DERIVATION_H
