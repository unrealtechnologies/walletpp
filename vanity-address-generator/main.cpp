//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "entropy.h"
#include "ethereum_utils.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include "constants.h"
#include "extended_key.h"

void findAddress(unsigned int start, unsigned int step) {
    while (true) {
        const auto entrop = entropy::generate_entropy(32);
        const auto words = bip39::mnemonic_from_entropy(entrop);
        const auto seed = bip39::seed_from_mnemonic(words);
        const auto b32 = bip32::from_entropy(seed);

        auto root_path_string = "m/44'/60'/0'/0";
        const auto node = b32.derive_keypair_with_path(root_path_string);

        for (size_t j = start; j < walletpp::hardened_key_start_index; j += step) {

            auto derived_node = node->derive_child(j);
            auto key_pair = derived_node->get_key_pair();

            if (const auto address = ethereum_utils::generate_ethereum_address(key_pair.private_key.key); address.contains("0x000000")) {
                for (auto word: words) {
                    std::cout << word << " ";
                }
                std::cout << std::endl;
                std::cout << address << " with index:" << j << std::endl;

                break; // Exit the loop if a matching address is found
            }

            node->remove_child(j);
        }
    }
}

int main() {
    unsigned int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < cores; ++i) {
        threads.emplace_back(findAddress, i, cores);
    }

    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
