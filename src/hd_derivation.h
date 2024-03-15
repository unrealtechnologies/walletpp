//
// Created by ariel on 3/11/24.
//

#ifndef HD_DERIVATION_H
#define HD_DERIVATION_H

#include "extended_key.h"
#include "key_pair.h"

class hd_derivation {

public:
    [[nodiscard]] static bool is_hardened_key(size_t index);
    [[nodiscard]] static extended_key private_child_key_derivation(const extended_key &parent_key, size_t index);
    [[nodiscard]] static extended_key public_child_key_derivation(const extended_key &parent_key, size_t index);
    [[nodiscard]] static key_pair private_and_public_key_pair_derivation(const extended_key &parent_key, size_t index);
};


#endif//HD_DERIVATION_H
