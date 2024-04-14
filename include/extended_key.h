//
// Created by ariel on 3/11/24.
//

#ifndef EXTENDED_KEY_H
#define EXTENDED_KEY_H

#include "constants.h"
#include "crypto_algorithms.h"

namespace walletpp {
    struct extended_key {
        secure_vector<uint8_t> key;
        secure_vector<uint8_t> chain_code;
        secure_vector<uint8_t> parent_finger_print;
        const uint8_t depth;
        const uint32_t index;
        const bool is_private_key;

        extended_key(secure_vector<uint8_t> &&key, secure_vector<uint8_t> &&chain_code, secure_vector<uint8_t> &&parent_finger_print, const uint8_t depth, const uint64_t index)
            : key(std::move(key)), chain_code(std::move(chain_code)), parent_finger_print(std::move(parent_finger_print)), depth(depth), index(index),
              is_private_key([this]() -> bool {
                  if (this->key.size() == private_key_bytes_length) { return true; }
                  if (this->key.size() == public_key_bytes_length) { return false; }
                  throw std::runtime_error("Incorrect key size");
              }()) {}

        [[nodiscard]] auto serialize() const -> secure_vector<uint8_t>;
        [[nodiscard]] auto to_base58_string() const -> std::string;
        [[nodiscard]] auto to_base58_vector() const -> secure_vector<uint8_t>;
        [[nodiscard]] static auto fingerprint(const secure_vector<uint8_t> &key) -> secure_vector<uint8_t>;
    };
}// namespace walletpp

#endif//EXTENDED_KEY_H
