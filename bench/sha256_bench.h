//
// Created by Ariel Saldana on 3/28/24.
//

#ifndef SHA256_BENCH_H
#define SHA256_BENCH_H

#include "crypto_algorithms.h"
class sha256_bench {
public:
    static void bench() {
        auto rounds = 300'000;
        volatile uint8_t dummy;

        for (auto i = 0; i < rounds; i++) {
            auto digest = walletpp::crypto_algorithms::sha256(walletpp::crypto_algorithms::generate_entropy(32));
            dummy = digest[0];
        }
    }
};

#endif //SHA256_BENCH_H
