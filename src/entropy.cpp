//
// Created by ariel on 3/8/24.
//

#include "entropy.h"

Botan::secure_vector<uint8_t> entropy::generate_entropy(uint32_t byte_size) {
    static Botan::AutoSeeded_RNG rng;
    // Generate a vector of random bytes
    Botan::secure_vector<uint8_t> random_data(byte_size);
    rng.randomize(random_data.data(), random_data.size());

    return random_data;
}
