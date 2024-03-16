//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "entropy.h"
#include "ethereum_utils.h"

#include <iostream>
#include <thread>
#include <vector>

void findAddress() {
    while (true) {
        const auto entrop = entropy::generate_entropy(32);
        const auto words = bip39::mnemonic_from_entropy(entrop);
        const auto seed = bip39::seed_from_mnemonic(words);
        const auto b32 = bip32::from_entropy(seed);

        const auto key_pair_address_0 = b32.derive_keypair_with_path("m/44'/60'/0'/0/0");
        const auto address_0 = ethereum_utils::generate_ethereum_address(key_pair_address_0.private_key.key);

        if (address_0.contains("0x00000000")) {
            for (auto word: words) {
                std::cout << word << " ";
            }
            std::cout << std::endl;
            std::cout << address_0 << std::endl;
            break; // Exit the loop if a matching address is found
        }
    }
}

int main() {
    unsigned int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < cores; ++i) {
        threads.emplace_back(findAddress);
    }

    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
