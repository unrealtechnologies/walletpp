//
// Created by ariel on 3/8/24.
//

#include "entropy.h"
#include <botan/auto_rng.h>
#include <iostream>
#include <duthomhas/csprng.hpp>

namespace walletpp {
    secure_vector<uint8_t> entropy::generate_entropy(uint32_t byte_size) {
        duthomhas::csprng rng;
        // auto random_data = rng( secure_vector<uint8_t> ( byte_size ) );
        walletpp::secure_vector<uint8_t> random_data(byte_size); // Create a vector with the desired size first
        rng(random_data); // Then pass it to the rng

        return random_data;
    }
}// namespace walletpp
