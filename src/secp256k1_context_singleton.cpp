//
// Created by Ariel Saldana on 3/11/24.
//

#include "secp256k1_context_singleton.h"
#include "entropy.h"
#include <stdexcept>

namespace walletpp {
    std::mutex secp256k1_context_singleton::mutex;

    secp256k1_context_singleton::secp256k1_context_singleton() {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
        if (!secp256k1_context_randomize(ctx, entropy::generate_entropy(32).data())) {
            // this if is more of a way to silence the warning about the return value of randomize
            throw std::runtime_error("Failed to randomize secp256k1 context");
        }
    }

    secp256k1_context *secp256k1_context_singleton::get_secp256k1_context() const { return ctx; }

    secp256k1_context_singleton &secp256k1_context_singleton::get_instance() {
        std::lock_guard lock(mutex);
        static secp256k1_context_singleton instance;
        return instance;
    }
}// namespace walletpp
