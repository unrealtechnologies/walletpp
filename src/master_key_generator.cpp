//
// Created by Ariel Saldana on 3/13/24.
//

#include "master_key_generator.h"

std::unique_ptr<extended_key> walletpp::master_key_generator::generate_private_key(const Botan::secure_vector<uint8_t> &seed) {
    std::string key_string = "Bitcoin seed";
    const Botan::secure_vector<uint8_t> key{key_string.begin(), key_string.end()};
    auto hash_output = crypto_algorithms::hmac512(seed, key);

    Botan::secure_vector<uint8_t> m_key = {hash_output.begin(), hash_output.begin() + private_key_bytes_length};
    Botan::secure_vector<uint8_t> m_chaincode = {hash_output.begin() + chaincode_byte_lenth, hash_output.end()};
    Botan::secure_vector<uint8_t> parent_finger_print = {zero_byte, zero_byte, zero_byte, zero_byte};
    auto extended_master_key = std::make_unique<extended_key>(m_key, m_chaincode, parent_finger_print, zero_byte, zero_byte);

    return extended_master_key;
}
std::unique_ptr<extended_key> walletpp::master_key_generator::generate_public_key(const extended_key &master_private_extended_key) {
    auto public_key = crypto_algorithms::generate_public_key(master_private_extended_key.key, true);
    auto extended_master_public_key = std::make_unique<extended_key>(
            public_key,
            master_private_extended_key.chain_code,
            master_private_extended_key.parent_finger_print,
            master_private_extended_key.depth, master_private_extended_key.index);

    extended_master_public_key->serialize();

    return extended_master_public_key;
}
