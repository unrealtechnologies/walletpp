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

    static constexpr size_t hardened_start_index = 0x80000000;

public:
    static bool is_hardened_key(size_t index) {
        return index >= hardened_start_index;
    }

    static void child_key_derivation(const extended_key &parent_key, size_t index) {
        auto is_hardered = is_hardened_key(index);

        // lets just start with the private derivation
    }

    static extended_key private_child_key_derivation(const extended_key &parent_key, size_t index) {
        const auto is_hardered = is_hardened_key(index);
        const auto public_key = crypto_algorithms::generate_public_key(parent_key.key, true);
        Botan::secure_vector<uint8_t> output{};

        auto index_be_format = crypto_algorithms::uint32_to_big_endian_bytes(index);
        if (is_hardered) {
            Botan::secure_vector<uint8_t> p_key = {walletpp::zero_byte};
            p_key.insert(p_key.end(), parent_key.key.begin(), parent_key.key.end());
            p_key.insert(p_key.end(), index_be_format.begin(), index_be_format.end());
            output = crypto_algorithms::hmac512(parent_key.chain_code, p_key);
        } else {
            Botan::secure_vector<uint8_t> p_key{};
            p_key.insert(p_key.end(), public_key.begin(), public_key.end());
            p_key.insert(p_key.end(), index_be_format.begin(), index_be_format.end());
            output = crypto_algorithms::hmac512(p_key, parent_key.chain_code);
        }

        const Botan::secure_vector<uint8_t> IL = {output.begin(), output.begin() + walletpp::private_key_bytes_length};
        const Botan::secure_vector<uint8_t> IR = {output.begin() + walletpp::private_key_bytes_length, output.end()};

        //The returned child key ki is parse256(IL) + kpar (mod n).
        const auto private_key = crypto_algorithms::generate_private_key(parent_key.key, IL);
        const auto fingerprint = extended_key::fingerprint(public_key);
        //todo: evaluate if we should return unique_ptrs like below
        //auto extended_private_key = std::make_unique<extended_key>(
        auto extended_private_key = extended_key(
                private_key,
                IR,
                fingerprint,
                parent_key.depth + 1,
                index);

        return extended_private_key;
    }
};


#endif//HD_DERIVATION_H
