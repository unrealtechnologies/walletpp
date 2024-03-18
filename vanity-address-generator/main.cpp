//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "constants.h"
#include "entropy.h"
#include "ethereum_utils.h"
#include "extended_key.h"
#include <fstream>
#include <thread>
#include <vector>

void findAddress(unsigned int start, unsigned int step, const std::string &file) {
    std::ofstream outfile;
    outfile.open(file, std::ios_base::app);
    while (true) {
        const auto entrop = entropy::generate_entropy(32);
        const auto words = bip39::mnemonic_from_entropy(entrop);
        const auto seed = bip39::seed_from_mnemonic(words);
        const auto b32 = bip32::from_entropy(seed);

        const auto root_path_string = "m/44'/60'/0'/0";
        const auto node = b32.derive_keypair_with_path(root_path_string);

        for (size_t j = start; j < walletpp::hardened_key_start_index; j += step) {

            const auto derived_node = node->derive_child(j);
            const auto key_pair = derived_node->get_key_pair();

            if (const auto address = ethereum_utils::generate_ethereum_address(key_pair.private_key.key); address.contains("0x00000000")) {
                std::stringstream worr_stream;
                for (auto word: words) {
                    worr_stream << word << " ";
                }

                outfile << worr_stream.str() << std::endl;
                outfile << address << std::endl;
            }

            node->remove_child(j);
        }
    }
}

int main() {
    const std::string log_file = "./addresses-rare.txt";

    unsigned int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < cores; ++i) {
        threads.emplace_back(findAddress, i, cores, log_file);
    }

    for (auto &th: threads) {
        th.join();
    }

    return 0;
}
