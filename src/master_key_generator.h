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
        [[nodiscard]] static auto generate_private_key(const secure_vector<uint8_t> &seed) -> extended_key;
        [[nodiscard]] static auto generate_public_key(const extended_key &master_private_extended_key) -> extended_key;
        [[nodiscard]] static auto generate_master_key_pair(const secure_vector<uint8_t> &seed) -> key_pair;
    };
}// namespace walletpp


#endif//MASTER_KEY_GENERATOR_H
