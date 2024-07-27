#ifndef DECRYPT_FILE_H
#define DECRYPT_FILE_H

#include "encryption_vars.h"
#include <fstream>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <vector>
#include <string>

// Function to read file contents into a vector
inline std::vector<unsigned char> read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Decrypt AES key using RSA private key
inline std::vector<unsigned char> decrypt_aes_key(const std::string& private_key_file, const std::vector<unsigned char>& encrypted_key) {
    FILE* privkey_file = fopen(private_key_file.c_str(), "r");
    if (!privkey_file) {
        std::cerr << "Unable to open private key file" << std::endl;
        handle_errors();
    }

    EVP_PKEY* privkey = PEM_read_PrivateKey(privkey_file, NULL, NULL, NULL);
    fclose(privkey_file);
    if (!privkey) {
        std::cerr << "Unable to read private key" << std::endl;
        handle_errors();
    }

    EVP_PKEY_CTX* pkey_ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!pkey_ctx) {
        EVP_PKEY_free(privkey);
        handle_errors();
    }

    if (EVP_PKEY_decrypt_init(pkey_ctx) <= 0) {
        EVP_PKEY_CTX_free(pkey_ctx);
        EVP_PKEY_free(privkey);
        handle_errors();
    }

    size_t decrypted_key_len;
    if (EVP_PKEY_decrypt(pkey_ctx, NULL, &decrypted_key_len, encrypted_key.data(), encrypted_key.size()) <= 0) {
        EVP_PKEY_CTX_free(pkey_ctx);
        EVP_PKEY_free(privkey);
        handle_errors();
    }

    std::vector<unsigned char> decrypted_key(decrypted_key_len);
    if (EVP_PKEY_decrypt(pkey_ctx, decrypted_key.data(), &decrypted_key_len, encrypted_key.data(), encrypted_key.size()) <= 0) {
        EVP_PKEY_CTX_free(pkey_ctx);
        EVP_PKEY_free(privkey);
        handle_errors();
    }

    EVP_PKEY_CTX_free(pkey_ctx);
    EVP_PKEY_free(privkey);

    return decrypted_key;
}

inline std::string decrypt(const std::vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv, const std::vector<unsigned char>& tag) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handle_errors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
        handle_errors();

    if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv))
        handle_errors();

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;

    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()))
        handle_errors();

    int plaintext_len = len;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_GCM_TAG_SIZE, const_cast<unsigned char*>(tag.data())))
        handle_errors();

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len))
        handle_errors();

    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);

    return std::string(plaintext.begin(), plaintext.end());
}

#endif // DECRYPT_FILE_H
