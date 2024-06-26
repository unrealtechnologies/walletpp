//
// Created by Ariel Saldana on 3/13/24.
//

#include "master_key_generator.h"

namespace walletpp {
    auto master_key_generator::generate_private_key(const secure_vector<uint8_t> &seed) -> std::unique_ptr<extended_key> {
        const std::string key_string = "Bitcoin seed";
        const secure_vector<uint8_t> key{key_string.begin(), key_string.end()};
        const auto hash_output = crypto_algorithms::hmac512(seed, key);

        secure_vector<uint8_t> m_key = {hash_output.begin(), hash_output.begin() + private_key_bytes_length};
        secure_vector<uint8_t> m_chaincode = {hash_output.begin() + chaincode_byte_length, hash_output.end()};
        auto empty_vec = secure_vector<uint8_t>();
        auto extended_master_key = std::make_unique<extended_key>(extended_key(std::move(m_key), std::move(m_chaincode), std::move(empty_vec), zero_byte, zero_byte));

        return extended_master_key;
    }

    auto master_key_generator::generate_public_key(const extended_key &master_private_extended_key) -> std::unique_ptr<extended_key> {
        auto public_key = crypto_algorithms::generate_public_key(master_private_extended_key.key, true);
        auto copy_of_chaincode = master_private_extended_key.chain_code;
        auto copy_of_fingerprint = master_private_extended_key.parent_finger_print;
        auto extended_master_public_key = std::make_unique<extended_key>(extended_key(std::move(public_key), std::move(copy_of_chaincode), std::move(copy_of_fingerprint),
                                                                                      master_private_extended_key.depth, master_private_extended_key.index));

        return extended_master_public_key;
    }

    auto master_key_generator::generate_master_key_pair(const secure_vector<uint8_t> &seed) -> std::pair<std::unique_ptr<extended_key>, std::unique_ptr<extended_key>> {
        auto private_key = generate_private_key(seed);
        auto public_key = generate_public_key(*private_key);
        auto kp = std::make_pair<std::unique_ptr<extended_key>, std::unique_ptr<extended_key>>(std::move(private_key), std::move(public_key));

        return kp;
    }

}// namespace walletpp
