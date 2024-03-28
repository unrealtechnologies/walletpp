//
// Created by ariel on 3/11/24.
//

#include "hd_derivation.h"

namespace walletpp {
    bool hd_derivation::is_hardened_key(const size_t index) { return index >= hardened_key_start_index; }

    auto hd_derivation::private_child_key_derivation(const extended_key &parent_key, const size_t index) noexcept -> extended_key {
        if (parent_key.key.size() != private_key_bytes_length) {
            throw std::runtime_error("The private key should be 256 bits, are you attempting to generate the pair with a public key?");
        }
        const auto is_hardered = is_hardened_key(index);
        const auto parent_public_key = crypto_algorithms::generate_public_key(parent_key.key, true);
        const auto index_be_format = crypto_algorithms::uint32_to_big_endian_bytes(index);

        secure_vector<uint8_t> output{};
        if (is_hardered) {
            secure_vector<uint8_t> p_key = {zero_byte};
            p_key.insert(p_key.end(), parent_key.key.begin(), parent_key.key.end());
            p_key.insert(p_key.end(), index_be_format.begin(), index_be_format.end());
            output = crypto_algorithms::hmac512(p_key, parent_key.chain_code);
        } else {
            secure_vector<uint8_t> p_key{};
            p_key.insert(p_key.end(), parent_public_key.begin(), parent_public_key.end());
            p_key.insert(p_key.end(), index_be_format.begin(), index_be_format.end());
            output = crypto_algorithms::hmac512(p_key, parent_key.chain_code);
        }

        const secure_vector<uint8_t> IL = {output.begin(), output.begin() + private_key_bytes_length};
        secure_vector<uint8_t> IR = {output.begin() + private_key_bytes_length, output.end()};

        //The returned child key ki is parse256(IL) + kpar (mod n).
        auto private_key = crypto_algorithms::generate_private_key(parent_key.key, IL);
        auto fingerprint = extended_key::fingerprint(parent_public_key);
        auto extended_private_key = extended_key(std::move(private_key), std::move(IR), std::move(fingerprint), parent_key.depth + 1, index);

        return extended_private_key;
    }

    auto hd_derivation::public_child_key_derivation(const extended_key &parent_key, const size_t index) noexcept -> extended_key {
        const auto private_key = private_child_key_derivation(parent_key, index);
        auto public_key = crypto_algorithms::generate_public_key(private_key.key, true);
        auto copied_chain_code = private_key.chain_code;
        auto fingerprint = extended_key::fingerprint(public_key);
        auto extended_public_key = extended_key(std::move(public_key), std::move(copied_chain_code), std::move(fingerprint), private_key.depth + 1, index);

        return extended_public_key;
    }

    auto hd_derivation::private_and_public_key_pair_derivation(const extended_key &parent_key, const size_t index) noexcept -> key_pair {
        if (parent_key.key.size() != private_key_bytes_length) {
            throw std::runtime_error("The private key should be 256 bits, are you attempting to generate the pair with a public key?");
        }
        auto extended_private_key = private_child_key_derivation(parent_key, index);
        auto copied_extended_private_key_chaincode = extended_private_key.chain_code;
        auto copied_extended_private_key_parent_finger_print = extended_private_key.parent_finger_print;
        auto public_key = crypto_algorithms::generate_public_key(extended_private_key.key, true);
        auto extended_public_key = extended_key(std::move(public_key), std::move(copied_extended_private_key_chaincode), std::move(copied_extended_private_key_parent_finger_print),
                                                extended_private_key.depth, extended_private_key.index);

        return {std::move(extended_private_key), std::move(extended_public_key)};
    }
}// namespace walletpp
