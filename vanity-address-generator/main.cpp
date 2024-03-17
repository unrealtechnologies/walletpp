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

void findAddress() {
    while (true) {
        const auto entrop = entropy::generate_entropy(32);
        const auto words = bip39::mnemonic_from_entropy(entrop);
        const auto seed = bip39::seed_from_mnemonic(words);
        const auto b32 = bip32::from_entropy(seed);

        // const auto root_node = b32.derive_keypair_with_path("m/44'/60'/0'/0");
        for (auto j = 0; j < walletpp::hardened_key_start_index; j++) {
            std::ostringstream path_stream;
            path_stream << "m/44'/60'/0'/0/" << j;
            const auto path_string = path_stream.str();
            const auto key_pair_address = b32.derive_keypair_with_path(path_string);

            if (const auto address = ethereum_utils::generate_ethereum_address(key_pair_address.private_key.key); address.contains("0x00000000")) {
                for (auto word: words) {
                    std::cout << word << " ";
                }
                std::cout << std::endl;
                std::cout << address << " with index:" << j << std::endl;

                break; // Exit the loop if a matching address is found
            }
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
