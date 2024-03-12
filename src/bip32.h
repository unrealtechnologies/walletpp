//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef BIP32_H
#define BIP32_H

#include "extended_key.h"
#include <array>
#include <cstddef>

class bip32 {
public:
    constexpr static size_t bytes_x32 = 256;
    constexpr static size_t bytes_x64 = 512;
    [[nodiscard]] static std::unique_ptr<extended_key> generate_master_key(std::array<uint8_t, bytes_x32> seed);
};


#endif//BIP32_H
