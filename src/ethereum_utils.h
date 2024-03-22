//
// Created by Ariel Saldana on 3/16/24.
//

#ifndef ETHEREUM_UTILS_H
#define ETHEREUM_UTILS_H

#include "crypto_algorithms.h"
#include <botan/hex.h>

namespace walletpp {
    class ethereum_utils {
    public:
        [[nodiscard]] static auto generate_ethereum_address(const Botan::secure_vector<uint8_t> &key) -> std::string;
        [[nodiscard]] static auto to_checksum_address(const std::string &address) -> std::string;
    };
}// namespace walletpp


#endif//ETHEREUM_UTILS_H
