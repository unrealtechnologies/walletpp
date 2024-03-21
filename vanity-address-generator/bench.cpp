//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "entropy.h"
#include "ethereum_utils.h"

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

constexpr size_t NUM_ADDRESSES = 1'000'000;

void findAddresses(size_t num_addresses) {
    for (size_t i = 0; i < num_addresses; ++i) {
        const auto entrop = entropy::generate_entropy(32);
        const auto words = bip39::mnemonic_from_entropy(entrop);
        const auto seed = bip39::seed_from_mnemonic(words);
        const auto b32 = bip32::from_entropy(seed);

        const auto key_pair_address_0 = b32.derive_keypair_with_path("m/44'/60'/0'/0/0");
        const auto address_0 = ethereum_utils::generate_ethereum_address(key_pair_address_0.private_key.key);
    }
}

int main() {
    unsigned int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    size_t addresses_per_thread = NUM_ADDRESSES / cores;

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < cores; ++i) {
        threads.emplace_back(findAddresses, addresses_per_thread);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Generated " << NUM_ADDRESSES << " addresses in " << elapsed.count() << " seconds." << std::endl;

    return 0;
}
