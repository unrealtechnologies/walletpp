//
// Created by Ariel Saldana on 3/28/24.
//

#ifndef KECCAK256_BENCH_H
#define KECCAK256_BENCH_H

#include "crypto_algorithms.h"

class keccak256 {
    public:
    static void bench() {
        auto rounds = 1'000'000;
        volatile uint8_t dummy;

        std::string message = "aefbb50942817d8270bb9bd922aa5ca9cb06cdbf";
        for (auto i = 0; i < rounds; i++) {
            auto address = walletpp::crypto_algorithms::keccak256(message);
            dummy = address[0];
            // std::cout << i << dummy << std::endl;
        }
    }
};

#endif //KECCAK256_BENCH_H
