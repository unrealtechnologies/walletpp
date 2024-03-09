//
// Created by ariel on 3/9/24.
//

#include "crypto_algorithms.h"
#include <botan/hex.h>

std::vector<uint8_t> crypto_algorithms::sha256(const std::vector<uint8_t> &contents) {
    const auto hash1 = Botan::HashFunction::create_or_throw("SHA-256");
    hash1->update(contents.data(), contents.size());
    auto secure_msg =  hash1->final();
    return { secure_msg.begin(), secure_msg.end()};
}

std::string crypto_algorithms::to_hex(const std::vector<uint8_t> &contents) {
    return Botan::hex_encode(contents);
}

std::vector<uint8_t> crypto_algorithms::from_hex(const std::string &hex_string) {
    return Botan::hex_decode(hex_string);
}


