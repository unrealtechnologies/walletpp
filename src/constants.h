//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "secure_vector.h"
#include <botan/secmem.h>
#include <cstddef>
#include <cstdint>
#include <iostream>


namespace walletpp {
    static constexpr size_t private_key_bytes_length = 32;
    static constexpr size_t public_key_bytes_length = 33;
    static constexpr size_t chaincode_byte_length = 32;
    static constexpr size_t serialized_extended_key_bytes_length = 82;
    static constexpr size_t base58_serialized_extened_key_size = 112;
    static constexpr size_t extended_key_version_byte_length = 4;
    static constexpr size_t fingerprint_byte_length = 4;
    static constexpr uint8_t zero_byte = 0x00;
    static constexpr uint8_t public_key_uncompressed_bytes_length = 65;
    static constexpr uint8_t public_key_uncompressed_form_id = 0x04;
    static constexpr size_t hardened_key_start_index = 0x80000000;
    constexpr static uint16_t sha256_output_byte_size = 32;
    constexpr static uint16_t default_pbkdf2_iterations = 2048;
    constexpr static uint16_t pbkdf2_sha512_output_byte_size = 64;
    static constexpr std::string_view default_mnemonic_salt_string = "mnemonic";
    static const walletpp::secure_vector<uint8_t> master_key_parent_key{0x00, 0x00, 0x00, 0x00};
    typedef walletpp::secure_vector<uint8_t> bytes_buffer;
    typedef uint8_t byte;
}// namespace walletpp

#endif//CONSTANTS_H
