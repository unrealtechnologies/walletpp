//
// Created by ariel on 3/9/24.
//

#include "crypto_algorithms.h"
#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <botan/pwdhash.h>


std::array<uint8_t, crypto_algorithms::sha256_output_byte_size> crypto_algorithms::sha256(const Botan::secure_vector<uint8_t> &contents) {
    const auto hash1 = Botan::HashFunction::create_or_throw("SHA-256");
    hash1->update(contents.data(), contents.size());
    Botan::secure_vector<uint8_t> secure_msg = hash1->final();

    // Initialize std::array and copy the std::vector
    std::array<uint8_t, crypto_algorithms::sha256_output_byte_size> arrayData{};
    std::ranges::copy(secure_msg, arrayData.begin());

    return arrayData;
}

std::string crypto_algorithms::to_hex(const Botan::secure_vector<uint8_t> &contents) {
    return Botan::hex_encode(contents);
}

Botan::secure_vector<uint8_t> crypto_algorithms::from_hex(const std::string &hex_string) {
    std::vector<uint8_t> decoded_msg = Botan::hex_decode(hex_string);
    Botan::secure_vector<uint8_t> raw_bytes(decoded_msg.begin(), decoded_msg.end());

    return raw_bytes;
}

std::array<uint8_t, crypto_algorithms::pbkdf2_sha512_output_byte_size> crypto_algorithms::pbkdf2(const Botan::secure_vector<uint8_t> &password, const Botan::secure_vector<uint8_t> &salt) {
    constexpr std::string_view salt_string = "mnemonic";
    Botan::secure_vector<uint8_t> salt_vector(salt_string.begin(), salt_string.end());
    salt_vector.insert(salt_vector.end(), salt.begin(), salt.end());

    auto pwd_fam = Botan::PasswordHashFamily::create_or_throw(pbkdf2_algorithm);
    auto pwd_hash = pwd_fam->from_iterations(pbkdf2_iterations);

    // std::array<uint8_t, 32> salt;
    std::array<uint8_t, pbkdf2_sha512_output_byte_size> key{};
    pwd_hash->derive_key(key.data(), key.size(), reinterpret_cast<const char *>(password.data()), password.size(), salt_vector.data(), salt_vector.size());

    return key;
}
std::array<uint8_t, crypto_algorithms::pbkdf2_sha512_output_byte_size> crypto_algorithms::pbkdf2(const std::string_view password, const std::string_view salt) {
    const auto pwd_fam = Botan::PasswordHashFamily::create_or_throw(pbkdf2_algorithm);
    const auto pwd_hash = pwd_fam->from_iterations(pbkdf2_iterations);
    const Botan::secure_vector<uint8_t> salt_vector(salt.begin(), salt.end());

    // std::array<uint8_t, 32> salt;
    std::array<uint8_t, pbkdf2_sha512_output_byte_size> key{};
    pwd_hash->hash(key, password, salt_vector);
    pwd_hash->derive_key(key.data(), key.size(), reinterpret_cast<const char *>(password.data()), password.size(), salt_vector.data(), salt_vector.size());

    return key;
}
Botan::secure_vector<bool> crypto_algorithms::binary_from_bytes(const Botan::secure_vector<uint8_t> &bytes, const std::optional<size_t> &num_of_bits) {
    // we can use a bool to represent bits (0s and 1s) true is 1, false is 0
    Botan::secure_vector<bool> bits;
    for (const auto byte: bytes) {
        std::bitset<8> bitSet(byte);
        for (int i = 7; i >= 0; --i) {// Start from the most significant bit
            bits.push_back(bitSet.test(i) ? true : false);
        }
    }

    if (num_of_bits) {
        return {bits.begin(), bits.begin() + num_of_bits.value()};
    }
    return bits;
}
