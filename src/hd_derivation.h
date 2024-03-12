//
// Created by ariel on 3/11/24.
//

#ifndef HD_DERIVATION_H
#define HD_DERIVATION_H

#include "extended_key.h"
#include <array>


class hd_derivation {
    constexpr static size_t version_array_length = 4;
    const std::array<uint8_t, version_array_length> privateVersion = {0x04, 0x88, 0xAD, 0xE4};
    const std::array<uint8_t, version_array_length> publicVersion = {0x04, 0x88, 0xB2, 0x1E};

    static constexpr size_t hardened_start_index = 0x80000000;

    static bool is_hardened_key(size_t index) {
        return index >= hardened_start_index;
    }

    static void child_key_derivation(const extended_key &parent_key, size_t index) {
        auto is_hardered = is_hardened_key(index);

        // lets just start with the private derivation

    }
};


#endif//HD_DERIVATION_H
