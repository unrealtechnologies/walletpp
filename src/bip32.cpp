//
// Created by Ariel Saldana on 3/12/24.
//

#include "bip32.h"
#include "crypto_algorithms.h"
#include <string>

std::unique_ptr<extended_key> bip32::generate_master_key(const Botan::secure_vector<uint8_t>& seed) {
    std::string key_string = "Bitcoin seed";
    const Botan::secure_vector<uint8_t> key{key_string.begin(), key_string.end()};
    auto hash_output = crypto_algorithms::hmac512(seed, key);

    Botan::secure_vector<uint8_t> m_key = {hash_output.begin(), hash_output.begin() + 32};
    Botan::secure_vector<uint8_t> m_chaincode = {hash_output.begin() + 32, hash_output.end()};
    Botan::secure_vector<uint8_t> parent_finger_print = {0x00, 0x00, 0x00, 0x00};
    auto extended_master_key = std::make_unique<extended_key>(m_key, m_chaincode, parent_finger_print, 0, 0);

    extended_master_key->serialize();

    return extended_master_key;
}
