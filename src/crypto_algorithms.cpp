//
// Created by ariel on 3/9/24.
//

#include "crypto_algorithms.h"
#include "secp256k1_context_singleton.h"
#include "secure_vector.h"
#include <cassert>
#include <cstring>
#include <memory>
#include <openssl/hmac.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "fastpbkdf2.h"
#include "libbase58.h"
#include "sha3.h"
#ifdef __cplusplus
}
#endif

#include <openssl/evp.h>
#include <openssl/sha.h>

namespace walletpp {
    secure_vector<uint8_t> crypto_algorithms::sha256(const secure_vector<uint8_t> &contents) {
        EVP_MD_CTX *ctx;
        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_len;

        if ((ctx = EVP_MD_CTX_new()) == nullptr) throw std::runtime_error("Failed to create new EVP_MD_CTX");

        if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) throw std::runtime_error("Failed to initialize digest");

        if (EVP_DigestUpdate(ctx, contents.data(), contents.size()) != 1) throw std::runtime_error("Failed to update digest");

        if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1) throw std::runtime_error("Failed to finalize digest");

        EVP_MD_CTX_free(ctx);

        secure_vector<uint8_t> digest_vector(digest, digest + digest_len);
        return {digest_vector.begin(), digest_vector.end()};
    }
    secure_vector<uint8_t> crypto_algorithms::double_sha256(const secure_vector<uint8_t> &contents) { return sha256(sha256(contents)); }

    secure_vector<uint8_t> crypto_algorithms::ripemd160(const secure_vector<uint8_t> &contents) {
        EVP_MD_CTX *mdctx;
        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len;

        if ((mdctx = EVP_MD_CTX_new()) == nullptr) { throw std::runtime_error("Failed to create new EVP_MD_CTX"); }

        if (1 != EVP_DigestInit_ex(mdctx, EVP_ripemd160(), nullptr)) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to initialize digest");
        }

        if (1 != EVP_DigestUpdate(mdctx, contents.data(), contents.size())) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update digest");
        }

        if (1 != EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to finalize digest");
        }

        EVP_MD_CTX_free(mdctx);

        secure_vector<uint8_t> digest_vector(md_value, md_value + md_len);
        return digest_vector;
    }

    secure_vector<uint8_t> crypto_algorithms::keccak256(const secure_vector<uint8_t> &contents) {
        sha3_context ctx;// Context initialization
        sha3_Init256(&ctx);
        sha3_Update(&ctx, contents.data(), contents.size());
        const auto phash_ptr = static_cast<const uint8_t *>(sha3_Finalize(&ctx));
        auto ret_vec = secure_vector<uint8_t>(phash_ptr, phash_ptr + 32);

        memset(&ctx, 0, sizeof(sha3_context));
        return ret_vec;
    }

    secure_vector<uint8_t> crypto_algorithms::keccak256(const std::string &contents) {
        sha3_context ctx;// Context initialization
        sha3_Init256(&ctx);
        sha3_Update(&ctx, contents.data(), contents.size());
        const auto phash_ptr = static_cast<const uint8_t *>(sha3_Finalize(&ctx));
        auto ret_vec = secure_vector<uint8_t>(phash_ptr, phash_ptr + 32);

        memset(&ctx, 0, sizeof(sha3_context));
        return ret_vec;
    }

    std::string crypto_algorithms::to_hex(const secure_vector<uint8_t> &v) {
        std::string result;
        result.reserve(v.size() * 2);// two digits per character

        static constexpr char hex[] = "0123456789ABCDEF";

        for (const uint8_t c: v) {
            result.push_back(hex[c / 16]);
            result.push_back(hex[c % 16]);
        }

        return result;
    }
    secure_vector<uint8_t> crypto_algorithms::from_hex(const std::string &hex_string) {
        secure_vector<uint8_t> bytes;

        for (unsigned int i = 0; i < hex_string.length(); i += 2) {
            std::string byteString = hex_string.substr(i, 2);
            auto byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
            bytes.emplace_back(byte);
        }

        return bytes;
    }

    secure_vector<uint8_t> crypto_algorithms::fast_pbkdf2(const std::string_view password, const std::string_view salt, const size_t iterations) {
        constexpr size_t key_length = pbkdf2_sha512_output_byte_size;
        secure_vector<uint8_t> out(key_length);
        fastpbkdf2_hmac_sha512(reinterpret_cast<const uint8_t *>(password.data()), password.size(), reinterpret_cast<const uint8_t *>(salt.data()), salt.size(), iterations,
                               out.data(), out.size());

        return out;
    }

    secure_vector<uint8_t> crypto_algorithms::binary_from_bytes(const secure_vector<uint8_t> &bytes, const std::optional<size_t> &num_of_bits) {
        secure_vector<uint8_t> bits;
        const size_t bits_to_process = num_of_bits.value_or(bytes.size() * 8);

        for (size_t byte_index = 0; byte_index < bytes.size() && bits.size() < bits_to_process; ++byte_index) {
            for (int bit_index = 7; bit_index >= 0 && bits.size() < bits_to_process; --bit_index) {
                // Extract each bit using bitwise AND and shift operations
                bool bit = (bytes[byte_index] & 1 << bit_index) != 0;
                bits.emplace_back(bit);
            }
        }

        return bits;
    }

    secure_vector<uint8_t> crypto_algorithms::hmac512(const secure_vector<uint8_t> &msg, const secure_vector<uint8_t> &key) {
        unsigned int len = EVP_MAX_MD_SIZE;
        secure_vector<uint8_t> digest(len);
        if (HMAC(EVP_sha512(), key.data(), static_cast<int>(key.size()), msg.data(), msg.size(), digest.data(), &len) == nullptr)
            throw std::runtime_error("Failed to compute HMAC-SHA512");
        digest.resize(len);// Resize the vector to fit the actual size of the HMAC

        return digest;
    }


    secure_vector<uint8_t> crypto_algorithms::generate_private_key(const secure_vector<uint8_t> &key, const secure_vector<uint8_t> &tweak) {
        const auto ctx = secp256k1_context_singleton::get_instance().get_secp256k1_context();

        // make a copy to tweak
        auto privateKey(key);

        if (!secp256k1_ec_seckey_tweak_add(ctx, privateKey.data(), tweak.data())) { throw std::runtime_error("Failed to derive child private key"); }

        if (!secp256k1_ec_seckey_verify(ctx, privateKey.data())) { throw std::runtime_error("Generated private key is invalid"); }

        return privateKey;
    }

    secure_vector<uint8_t> crypto_algorithms::generate_public_key(const secure_vector<uint8_t> &key, const bool compressed) {
        const auto ctx = secp256k1_context_singleton::get_instance().get_secp256k1_context();
        const auto len = compressed ? 33U : 65U;

        secp256k1_pubkey pubkey;

        const std::unique_ptr<unsigned char[]> publicKey33(new unsigned char[len + 1]);
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
        if (!secp256k1_ec_pubkey_create(ctx, &pubkey, key.data())) { throw std::runtime_error("Failed to create public key"); }

        /* Serialize Public Key */
        if (const auto serializationMethod = compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
            !secp256k1_ec_pubkey_serialize(ctx, publicKey33.get(), &pkLen, &pubkey, serializationMethod)) {
            throw std::runtime_error("Failed to serialize public key");
        }

        return {publicKey33.get(), publicKey33.get() + len};
    }

    uint32_t crypto_algorithms::htobe32(const uint32_t x) {
        union {
            uint32_t val;
            uint8_t bytes[4];
        } u{};
        u.val = x;
        secure_vector<uint8_t> vec(u.bytes, u.bytes + 4);
        std::ranges::reverse(vec);
        std::ranges::copy(vec, u.bytes);

        return u.val;
    }

    secure_vector<uint8_t> crypto_algorithms::uint32_to_big_endian_vector(const uint32_t num) {
        secure_vector<uint8_t> vec(4);
        const uint32_t be_num = htobe32(num);            // Convert to big-endian
        std::memcpy(vec.data(), &be_num, sizeof(be_num));// Copy to vector
        return vec;
    }
    secure_vector<uint8_t> crypto_algorithms::uint32_to_big_endian_bytes(const uint32_t value) {
        secure_vector<uint8_t> bytes(4);
        bytes[0] = static_cast<uint8_t>(value >> 24) & 0xFF;
        bytes[1] = static_cast<uint8_t>(value >> 16) & 0xFF;
        bytes[2] = static_cast<uint8_t>(value >> 8) & 0xFF;
        bytes[3] = static_cast<uint8_t>(value & 0xFF);

        return bytes;
    }
    auto crypto_algorithms::base58_encode_to_string(secure_vector<uint8_t> data) -> std::string {
        size_t b58sz = base58_serialized_extened_key_size;// Allocate more space for Base58 encoding
        std::vector<char> b58(b58sz);
        if (!b58enc(b58.data(), &b58sz, data.data(), data.size())) { throw std::runtime_error("Failed to encode to Base58"); }

        return {b58.begin(), b58.begin() + static_cast<long>(b58sz) - 1};
    }

    auto crypto_algorithms::base58_decode_to_vector(const std::string &base58_string) -> secure_vector<uint8_t> {
        const auto data = secure_vector<uint8_t>(base58_string.begin(), base58_string.end());
        auto binsz = base58_string.size() * 2;// Allocate more space for Base58 decoding
        std::vector<uint8_t> bin(binsz);
        if (!b58tobin(bin.data(), &binsz, base58_string.c_str(), data.size())) { throw std::runtime_error("Failed to decode from Base58"); }

        return {bin.begin(), bin.begin() + static_cast<long>(binsz)};
    }
}// namespace walletpp
