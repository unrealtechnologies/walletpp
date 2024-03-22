//
// Created by ariel on 3/8/24.
//

#ifndef ENTROPY_H
#define ENTROPY_H

#include <botan/auto_rng.h>
#include <vector>

namespace walletpp {
    class entropy {
    public:
        [[nodiscard]] static Botan::secure_vector<uint8_t> generate_entropy(uint32_t byte_size);
    };
}// namespace walletpp


#endif//ENTROPY_H
