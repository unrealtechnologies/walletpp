//
// Created by Ariel Saldana on 3/13/24.
//

#ifndef MASTER_KEY_GENERATOR_H
#define MASTER_KEY_GENERATOR_H

#include "crypto_algorithms.h"
#include "extended_key.h"
#include "key_pair.h"

namespace walletpp {
    class master_key_generator {
    public:
        [[nodiscard]] static std::unique_ptr<extended_key> generate_private_key(const Botan::secure_vector<uint8_t> &seed);
        [[nodiscard]] static std::unique_ptr<extended_key> generate_public_key(const extended_key &master_private_extended_key);
        [[nodiscard]] static std::unique_ptr<key_pair> generate_master_key_pair(const Botan::secure_vector<uint8_t> &seed);
    };
}// namespace walletpp


#endif//MASTER_KEY_GENERATOR_H