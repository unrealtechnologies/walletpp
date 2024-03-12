//
// Created by ariel on 3/11/24.
//

#ifndef EXTENDED_KEY_H
#define EXTENDED_KEY_H

#include <array>
#include <cstdint>
#include <cstddef>


struct extended_key {

private:
    constexpr static size_t chain_code_bytes_length = 32;
    constexpr static size_t key_bytes_length = 32;

public:
    bool is_private_key = true;
    std::array<uint8_t, key_bytes_length> key;
    std::array<uint8_t, chain_code_bytes_length> chain_code;
    std::array<uint8_t, 4> parentFingerprint;
    uint8_t depth;
    uint32_t index;
};

#endif//EXTENDED_KEY_H
