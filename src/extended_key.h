//
// Created by ariel on 3/11/24.
//

#ifndef EXTENDED_KEY_H
#define EXTENDED_KEY_H

#include "constants.h"
#include "crypto_algorithms.h"
#include <botan/hex.h>

namespace walletpp {
    struct extended_key {
        const Botan::secure_vector<uint8_t> key;
        const Botan::secure_vector<uint8_t> chain_code;
        const std::optional<Botan::secure_vector<uint8_t>> parent_finger_print;
        const uint8_t depth;
        const uint32_t index;
        const bool is_private_key;

        extended_key(
                const Botan::secure_vector<uint8_t> &key,
                const Botan::secure_vector<uint8_t> &chain_code,
                const std::optional<Botan::secure_vector<uint8_t>> &parent_finger_print,
                const uint8_t depth,
                const uint64_t index)
            : key(key),
              chain_code(chain_code),
              parent_finger_print(parent_finger_print),
              depth(depth),
              index(index),
              is_private_key([&]() -> bool {
                  if (key.size() == walletpp::private_key_bytes_length) {
                      return true;
                  }
                  if (key.size() == walletpp::public_key_bytes_length) {
                      return false;
                  }
                  throw std::runtime_error("Incorrect key size");
              }()) {}

        [[nodiscard]] auto serialize() const -> Botan::secure_vector<uint8_t>;
        [[nodiscard]] auto to_base58_string() const -> std::string;
        [[nodiscard]] auto to_base58_vector() const -> Botan::secure_vector<uint8_t>;
        [[nodiscard]] static auto fingerprint(const Botan::secure_vector<uint8_t> &key) -> Botan::secure_vector<uint8_t>;
    };
}// namespace walletpp

#endif//EXTENDED_KEY_H
