//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "entropy.h"
#include "ethereum_utils.h"
#include "keccak256_bench.h"
#include "address_generation_bench.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <vector>

constexpr size_t NUM_ADDRESSES = 1'000'000;

void findAddresses(size_t num_addresses) {
    for (size_t i = 0; i < num_addresses; ++i) {
        const auto entrop = walletpp::entropy::generate_entropy(32);
        const auto words = walletpp::bip39::mnemonic_from_entropy(entrop);
        const auto seed = walletpp::bip39::seed_from_mnemonic(words);
        const auto b32 = walletpp::bip32::from_seed(seed);

        const auto key_pair_address_0 = b32.derive_keypair_with_path("m/44'/60'/0'/0/0")->get_key_pair();
        const auto address_0 = walletpp::ethereum_utils::generate_ethereum_address(key_pair_address_0.private_key.key);
    }
}

#include "sha256_bench.h"

auto bench_this(const std::function < void()>& bench_func) -> void {
    auto start = std::chrono::high_resolution_clock::now();

    bench_func();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Benchmark took: "  << elapsed.count() << " seconds." << std::endl;
}

int main() {
    // bench_this(sha256_bench::bench);
    // bench_this(keccak256::bench);
    bench_this(address_generation_bench::benchmark);
    return 0;
}
