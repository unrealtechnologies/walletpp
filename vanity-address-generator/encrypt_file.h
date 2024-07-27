#ifndef ENCRYPT_FILE_H
#define ENCRYPT_FILE_H

#include "encryption_vars.h"
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <vector>

std::vector<unsigned char> encrypt(const std::string &public_key_path, const std::string &prefix, const std::string& plaintext, unsigned char* key, unsigned char* iv, std::vector<unsigned char>& tag) {
    // Generate key and IV
    if (!RAND_bytes(key, AES_256_KEY_SIZE) || !RAND_bytes(iv, AES_GCM_IV_SIZE)) {
        handle_errors();
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_errors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        handle_errors();
    }

    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        handle_errors();
    }

    std::vector<unsigned char> ciphertext(plaintext.size() + AES_GCM_TAG_SIZE);
    int len;

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        handle_errors();
    }

    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        handle_errors();
    }

    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    tag.resize(AES_GCM_TAG_SIZE);
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AES_GCM_TAG_SIZE, tag.data())) {
        EVP_CIPHER_CTX_free(ctx);
        handle_errors();
    }

    EVP_CIPHER_CTX_free(ctx);

    // Encrypt the AES key with RSA public key
    FILE* pubkey_file = fopen(public_key_path.c_str(), "r");
    if (!pubkey_file) {
        std::cerr << "Unable to open public key file" << std::endl;
        handle_errors();
    }

    EVP_PKEY* pubkey = PEM_read_PUBKEY(pubkey_file, NULL, NULL, NULL);
    fclose(pubkey_file);
    if (!pubkey) {
        std::cerr << "Unable to read public key" << std::endl;
        handle_errors();
    }

    EVP_PKEY_CTX* pkey_ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!pkey_ctx) {
        EVP_PKEY_free(pubkey);
        handle_errors();
    }

    if (EVP_PKEY_encrypt_init(pkey_ctx) <= 0) {
        EVP_PKEY_CTX_free(pkey_ctx);
        EVP_PKEY_free(pubkey);
        handle_errors();
    }

    size_t encrypted_key_len;
    if (EVP_PKEY_encrypt(pkey_ctx, NULL, &encrypted_key_len, key, AES_256_KEY_SIZE) <= 0) {
        EVP_PKEY_CTX_free(pkey_ctx);
        EVP_PKEY_free(pubkey);
        handle_errors();
    }

    std::vector<unsigned char> encrypted_key(encrypted_key_len);
    if (EVP_PKEY_encrypt(pkey_ctx, encrypted_key.data(), &encrypted_key_len, key, AES_256_KEY_SIZE) <= 0) {
        EVP_PKEY_CTX_free(pkey_ctx);
        EVP_PKEY_free(pubkey);
        handle_errors();
    }

    EVP_PKEY_CTX_free(pkey_ctx);
    EVP_PKEY_free(pubkey);

    // Securely save the encrypted key and IV
    std::ofstream keyfile(prefix + "_encrypted_key.bin", std::ios::binary);
    if (!keyfile) {
        std::cerr << "Unable to open file for writing: encrypted_key.bin" << std::endl;
        handle_errors();
    }
    keyfile.write(reinterpret_cast<const char*>(encrypted_key.data()), encrypted_key.size());
    keyfile.close();

    std::ofstream ivfile(prefix + "_iv.bin", std::ios::binary);
    if (!ivfile) {
        std::cerr << "Unable to open file for writing: iv.bin" << std::endl;
        handle_errors();
    }
    ivfile.write(reinterpret_cast<const char*>(iv), AES_GCM_IV_SIZE);
    ivfile.close();

    return ciphertext;
}

#endif // ENCRYPT_FILE_H
