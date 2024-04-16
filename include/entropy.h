//
// Created by ariel on 3/8/24.
//

#ifndef ENTROPY_H
#define ENTROPY_H

#include "secure_vector.h"
#include <cstdint>

namespace walletpp {
    class entropy {
    public:
        [[nodiscard]] static secure_vector<uint8_t> generate_entropy(size_t byte_size);
    };
}// namespace walletpp


#endif//ENTROPY_H
