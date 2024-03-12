//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <cstdint>

namespace walletpp {
    static constexpr size_t private_key_bytes_length = 32;
    static constexpr size_t public_key_bytes_length = 33;
    static constexpr size_t chaincode_byte_lenth = 32;
    static constexpr size_t serialized_extended_key_bytes_length = 82;
    static constexpr size_t extended_key_version_byte_length = 4;
    static constexpr uint8_t zero_byte = 0x00;
}// namespace walletpp

#endif//CONSTANTS_H
