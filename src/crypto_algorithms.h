//
// Created by ariel on 3/9/24.
//

#ifndef CRYPTO_ALGORITHMS_H
#define CRYPTO_ALGORITHMS_H

#include <vector>
#include <botan/hash.h>


struct crypto_algorithms {
    [[nodiscard]] static std::vector<uint8_t> sha256(const std::vector<uint8_t> &contents);
    [[nodiscard]] static std::string to_hex(const std::vector<uint8_t> & contents);
    [[nodiscard]] static std::vector<uint8_t> from_hex(const std::string &hex_string);
};


#endif//CRYPTO_ALGORITHMS_H
