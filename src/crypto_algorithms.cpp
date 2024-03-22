//
// Created by ariel on 3/9/24.
//

#include "crypto_algorithms.h"
#include "secp256k1.h"
#include "secp256k1_context_singleton.h"

#include <botan/auto_rng.h>
#include <botan/base58.h>
#include <botan/hex.h>
#include <botan/mac.h>
#include <botan/pwdhash.h>
#include <cassert>
#include <openssl/hmac.h>
#include <openssl/sha.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "fastpbkdf2/fastpbkdf2.h"
#ifdef __cplusplus
}
#endif


std::array<uint8_t, crypto_algorithms::sha256_output_byte_size> crypto_algorithms::sha256(const Botan::secure_vector<uint8_t> &contents) {
    const auto hash1 = Botan::HashFunction::create_or_throw("SHA-256");
    hash1->update(contents.data(), contents.size());
    Botan::secure_vector<uint8_t> secure_msg = hash1->final();

    // Initialize std::array and copy the std::vector
    std::array<uint8_t, sha256_output_byte_size> arrayData{};
    std::ranges::copy(secure_msg, arrayData.begin());

    return arrayData;
}
std::array<uint8_t, crypto_algorithms::sha256_output_byte_size> crypto_algorithms::double_sha256(const Botan::secure_vector<uint8_t> &contents) {
    const auto hash1 = Botan::HashFunction::create_or_throw("SHA-256");
    const auto hash2 = Botan::HashFunction::create_or_throw("SHA-256");
    hash1->update(contents.data(), contents.size());
    auto first_hash_data = hash1->final();
    hash2->update(first_hash_data.data(), first_hash_data.size());
    auto second_hash_data = hash2->final();

    std::array<uint8_t, sha256_output_byte_size> arrayData{};
    std::ranges::copy(second_hash_data, arrayData.begin());

    return arrayData;
}

Botan::secure_vector<uint8_t> crypto_algorithms::ripemd160(const Botan::secure_vector<uint8_t> &contents) {
    const auto hash = Botan::HashFunction::create_or_throw("RIPEMD-160");
    hash->update(contents);

    return hash->final();
}
Botan::secure_vector<uint8_t> crypto_algorithms::keccak256(const Botan::secure_vector<uint8_t> &contents) {
    const auto hash = Botan::HashFunction::create_or_throw("Keccak-1600(256)");
    hash->update(contents);

    return hash->final();
}

Botan::secure_vector<uint8_t> crypto_algorithms::keccak256(const std::string &contents) {
    const auto hash = Botan::HashFunction::create_or_throw("Keccak-1600(256)");
    hash->update(contents);

    return hash->final();
}

std::string crypto_algorithms::to_hex(const std::span<uint8_t> &contents) {
    return Botan::hex_encode(contents);
}

Botan::secure_vector<uint8_t> crypto_algorithms::from_hex(const std::string &hex_string) {
    std::vector<uint8_t> decoded_msg = Botan::hex_decode(hex_string);
    Botan::secure_vector<uint8_t> raw_bytes(decoded_msg.begin(), decoded_msg.end());

    return raw_bytes;
}

std::array<uint8_t, crypto_algorithms::pbkdf2_sha512_output_byte_size> crypto_algorithms::fast_pbkdf2(const std::string_view password, const std::string_view salt) {
    std::array<uint8_t, pbkdf2_sha512_output_byte_size> out{};

    fastpbkdf2_hmac_sha512(reinterpret_cast<const uint8_t *>(password.data()), password.size(),
                           reinterpret_cast<const uint8_t *>(salt.data()), salt.size(),
                           pbkdf2_iterations,
                           out.data(), out.size());

    return out;
}

Botan::secure_vector<bool> crypto_algorithms::binary_from_bytes(const Botan::secure_vector<uint8_t> &bytes, const std::optional<size_t> &num_of_bits) {
    Botan::secure_vector<bool> bits;
    size_t bits_to_process = num_of_bits.value_or(bytes.size() * 8);

    for (size_t byte_index = 0; byte_index < bytes.size() && bits.size() < bits_to_process; ++byte_index) {
        for (int bit_index = 7; bit_index >= 0 && bits.size() < bits_to_process; --bit_index) {
            // Extract each bit using bitwise AND and shift operations
            bool bit = (bytes[byte_index] & (1 << bit_index)) != 0;
            bits.emplace_back(bit);
        }
    }

    return bits;
}

Botan::secure_vector<uint8_t> crypto_algorithms::hmac512(const Botan::secure_vector<uint8_t> &msg, const Botan::secure_vector<uint8_t> &key) {
    auto hmac = Botan::MessageAuthenticationCode::create_or_throw("HMAC(SHA-512)");

    hmac->set_key(key);
    hmac->update(msg);
    auto hmacResult = hmac->final();

    // Copy the output to a new vector that uses the default allocator
    Botan::secure_vector<uint8_t> result(hmacResult.begin(), hmacResult.end());

    return result;
}

Botan::secure_vector<uint8_t> crypto_algorithms::generate_private_key(const Botan::secure_vector<uint8_t> &key, const Botan::secure_vector<uint8_t> &tweak) {
    const auto ctx = secp256k1_context_singleton::get_instance().get_secp256k1_context();

    // make a copy to tweak
    auto privateKey(key);

    if (!secp256k1_ec_seckey_tweak_add(ctx, privateKey.data(), tweak.data())) {
        throw std::runtime_error("Failed to derive child private key");
    }

    if (!secp256k1_ec_seckey_verify(ctx, privateKey.data())) {
        throw std::runtime_error("Generated private key is invalid");
    }

    return privateKey;
}

Botan::secure_vector<uint8_t> crypto_algorithms::generate_public_key(const Botan::secure_vector<uint8_t> &key, bool compressed) {
    const auto ctx = secp256k1_context_singleton::get_instance().get_secp256k1_context();
    const auto len = (compressed) ? 33U : 65U;

    secp256k1_pubkey pubkey;

    std::unique_ptr<unsigned char[]> publicKey33(new unsigned char[len + 1]);
    memset(publicKey33.get(), 0, len + 1);
    size_t pkLen = len + 1;

    /* Apparently there is a 2^-128 chance of
     * a secret key being invalid.
     * https://en.bitcoin.it/wiki/Private_key
     */
    /* Verify secret key is valid */
    if (!secp256k1_ec_seckey_verify(ctx, key.data())) {
        printf("Invalid secret key\n");
        throw std::runtime_error("Invalid secret key");
    }

    /* Create Public Key */
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, key.data())) {
        throw std::runtime_error("Failed to create public key");
    }

    auto serializationMethod = compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;

    /* Serialize Public Key */
    if (!secp256k1_ec_pubkey_serialize(ctx, publicKey33.get(), &pkLen, &pubkey,
                                       serializationMethod)) {
        throw std::runtime_error("Failed to serialize public key");
    }

    return {publicKey33.get(), publicKey33.get() + len};
}
Botan::secure_vector<uint8_t> crypto_algorithms::generate_entropy(const size_t bytes_size) {
    static Botan::AutoSeeded_RNG rng;
    // Generate a vector of random bytes
    Botan::secure_vector<uint8_t> random_data(bytes_size);
    rng.randomize(random_data.data(), random_data.size());

    return random_data;
}

uint32_t crypto_algorithms::htobe32(const uint32_t x) {
    union {
        uint32_t val;
        uint8_t bytes[4];
    } u{};
    u.val = x;
    std::vector<uint8_t> vec(u.bytes, u.bytes + 4);
    std::ranges::reverse(vec);
    std::ranges::copy(vec, u.bytes);

    return u.val;
}

Botan::secure_vector<uint8_t> crypto_algorithms::uint32_to_big_endian_vector(const uint32_t num) {
    Botan::secure_vector<uint8_t> vec(4);
    const uint32_t be_num = htobe32(num);            // Convert to big-endian
    std::memcpy(vec.data(), &be_num, sizeof(be_num));// Copy to vector
    return vec;
}
Botan::secure_vector<uint8_t> crypto_algorithms::uint32_to_big_endian_bytes(const uint32_t value) {
    Botan::secure_vector<uint8_t> bytes(4);
    bytes[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(value & 0xFF);

    return bytes;
}
