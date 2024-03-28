//
// Created by Ariel Saldana on 3/11/24.
//

#ifndef SECP256K1_CONTEXT_H
#define SECP256K1_CONTEXT_H

#include "secp256k1.h"
#include <mutex>

namespace walletpp {
    class secp256k1_context_singleton {
    public:
        static secp256k1_context_singleton &get_instance();
        [[maybe_unused]] [[nodiscard]] secp256k1_context *get_secp256k1_context() const;
        secp256k1_context_singleton(const secp256k1_context_singleton &) = delete;
        secp256k1_context_singleton &operator=(const secp256k1_context_singleton &) = delete;

    private:
        secp256k1_context *ctx;
        static std::mutex mutex;
        secp256k1_context_singleton();
    };
}// namespace walletpp

#endif//SECP256K1_CONTEXT_H
