//
// Created by Ariel Saldana on 3/16/24.
//

#include "extended_key.h"
#include <array>
#include <botan/base58.h>

namespace walletpp {
    auto extended_key::serialize() const -> walletpp::secure_vector<uint8_t> {
        walletpp::secure_vector<uint8_t> serialized_key;
        serialized_key.reserve(walletpp::serialized_extended_key_bytes_length);

        constexpr std::array<uint8_t, walletpp::extended_key_version_byte_length> private_version = {0x04, 0x88, 0xAD, 0xE4};
        constexpr std::array<uint8_t, walletpp::extended_key_version_byte_length> public_version = {0x04, 0x88, 0xB2, 0x1E};
        auto active_version = (key.size() == walletpp::private_key_bytes_length) ? private_version : public_version;
        serialized_key.insert(serialized_key.end(), active_version.begin(), active_version.end());
        serialized_key.insert(serialized_key.end(), depth);
        if (parent_finger_print.size() > 0) {
            serialized_key.insert(serialized_key.end(), parent_finger_print.begin(), parent_finger_print.end());
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

    std::string extended_key::to_base58_string() const {
        auto serialized_buff = serialize();
        auto base58_string = Botan::base58_encode(serialized_buff);
        return base58_string;
    }

    walletpp::secure_vector<uint8_t> extended_key::to_base58_vector() const {
        auto base58_string = to_base58_string();
        walletpp::secure_vector<uint8_t> base58_vec{base58_string.begin(), base58_string.end()};
        crypto_algorithms::secure_erase_string(base58_string);
        return base58_vec;
    }

    walletpp::secure_vector<uint8_t> extended_key::fingerprint(const walletpp::secure_vector<uint8_t> &key) {
        auto firstSha256 = crypto_algorithms::sha256(key);
        auto full_fingerprint = crypto_algorithms::ripemd160({firstSha256.begin(), firstSha256.end()});
        return {full_fingerprint.begin(), full_fingerprint.begin() + walletpp::fingerprint_byte_length};
    }
}
