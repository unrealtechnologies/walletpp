//
// Created by ariel on 3/11/24.
//

#ifndef EXTENDED_KEY_H
#define EXTENDED_KEY_H

#include "constants.h"
#include "crypto_algorithms.h"
#include <array>
#include <botan/base58.h>
#include <botan/hex.h>


struct extended_key {
    bool is_private_key;
    Botan::secure_vector<uint8_t> key;
    Botan::secure_vector<uint8_t> chain_code;
    std::optional<Botan::secure_vector<uint8_t>> parent_finger_print;
    uint8_t depth;
    uint32_t index;

    extended_key(
            const Botan::secure_vector<uint8_t> &key,
            const Botan::secure_vector<uint8_t> &chain_code,
            const std::optional<Botan::secure_vector<uint8_t>> &parent_finger_print,
            const uint8_t depth,
            const uint64_t index) {
        this->key = key;
        this->chain_code = chain_code;
        this->parent_finger_print = parent_finger_print;
        this->depth = depth;
        this->index = index;
        if (key.size() == walletpp::private_key_bytes_length) {
            is_private_key = true;
        } else if (key.size() == walletpp::public_key_bytes_length) {
            is_private_key = false;
        } else {
            throw std::runtime_error("Incorrect key size");
        }
    }

    Botan::secure_vector<uint8_t> serialize() {
        Botan::secure_vector<uint8_t> serialized_key;
        serialized_key.reserve(walletpp::serialized_extended_key_bytes_length);

        constexpr std::array<uint8_t, walletpp::extended_key_version_byte_length> private_version = {0x04, 0x88, 0xAD, 0xE4};
        constexpr std::array<uint8_t, walletpp::extended_key_version_byte_length> public_version = {0x04, 0x88, 0xB2, 0x1E};
        auto active_version = (key.size() == walletpp::private_key_bytes_length) ? private_version : public_version;
        serialized_key.insert(serialized_key.end(), active_version.begin(), active_version.end());
        serialized_key.insert(serialized_key.end(), depth);
        if (parent_finger_print.has_value()) {
            serialized_key.insert(serialized_key.end(), parent_finger_print.value().begin(), parent_finger_print.value().end());
        } else {
            serialized_key.insert(serialized_key.end(), walletpp::master_key_parent_key.begin(), walletpp::master_key_parent_key.end());
        }
        auto index_be_format = crypto_algorithms::uint32_to_big_endian_bytes(index);
        serialized_key.insert(serialized_key.end(), index_be_format.begin(), index_be_format.end());
        serialized_key.insert(serialized_key.end(), chain_code.begin(), chain_code.end());
        if (key.size() == walletpp::private_key_bytes_length) {
            serialized_key.emplace_back(walletpp::zero_byte);
        }
        serialized_key.insert(serialized_key.end(), key.begin(), key.end());
        auto checksum = crypto_algorithms::double_sha256(serialized_key);
        serialized_key.insert(serialized_key.end(), checksum.begin(), checksum.begin() + 4);

        return serialized_key;
    }

    std::string to_base58_string() {
        auto serialized_buff = serialize();
        auto base58_string = Botan::base58_encode(serialized_buff);
        return base58_string;
    }

    Botan::secure_vector<uint8_t> to_base58_vector() {
        auto base58_string = to_base58_string();
        Botan::secure_vector<uint8_t> base58_vec{base58_string.begin(), base58_string.end()};
        crypto_algorithms::secure_erase_string(base58_string);
        return base58_vec;
    }

    [[nodiscard]] static Botan::secure_vector<uint8_t> fingerprint(const Botan::secure_vector<uint8_t> &key) {
        auto firstSha256 = crypto_algorithms::sha256(key);
        auto full_fingerprint = crypto_algorithms::ripemd160({firstSha256.begin(), firstSha256.end()});
        return {full_fingerprint.begin(), full_fingerprint.begin() + walletpp::fingerprint_byte_length};
    }
};

#endif//EXTENDED_KEY_H
