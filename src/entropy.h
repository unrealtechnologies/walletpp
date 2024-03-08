//
// Created by ariel on 3/8/24.
//

#ifndef ENTROPY_H
#define ENTROPY_H

#include <vector>
#include <botan/auto_rng.h>

class entropy {

public:
    [[nodiscard]] static std::vector<uint8_t> generate_entropy(uint32_t byte_size);
};


#endif //ENTROPY_H
