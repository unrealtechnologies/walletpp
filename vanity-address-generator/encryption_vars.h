//
// Created by Ariel Saldana on 7/20/24.
//

#ifndef ENCRYPTION_VARS_H
#define ENCRYPTION_VARS_H
#include <openssl/err.h>

constexpr int AES_256_KEY_SIZE = 32;
constexpr int AES_GCM_IV_SIZE = 12;
constexpr int AES_GCM_TAG_SIZE = 16;

inline void handle_errors() {
    ERR_print_errors_fp(stderr);
    abort();
}

#endif //ENCRYPTION_VARS_H
