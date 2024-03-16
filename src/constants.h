//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <botan/secmem.h>
#include <cstddef>
#include <cstdint>

namespace walletpp {
    static constexpr size_t private_key_bytes_length = 32;
    static constexpr size_t public_key_bytes_length = 33;
    static constexpr size_t chaincode_byte_lenth = 32;
    static constexpr size_t serialized_extended_key_bytes_length = 82;
    static constexpr size_t extended_key_version_byte_length = 4;
    static constexpr size_t fingerprint_byte_length = 4;
    static constexpr uint8_t zero_byte = 0x00;
    static constexpr uint8_t public_key_uncompressed_bytes_length = 65;
    static constexpr uint8_t public_key_uncompressed_form_id = 0x04;
    static constexpr size_t hardened_key_start_index = 0x80000000;
    static const Botan::secure_vector<uint8_t> master_key_parent_key{0x00, 0x00, 0x00, 0x00};
    typedef Botan::secure_vector<uint8_t> bytes_buffer;
    typedef uint8_t byte;
}// namespace walletpp

#endif//CONSTANTS_H