//
// Created by Ariel Saldana on 3/28/24.
//

#include "bip32.h"
#include "entropy.h"
#include "ethereum_utils.h"
#include <chrono>
#include <thread>
#include <vector>

constexpr size_t NUM_ADDRESSES = 1'000'000;
class address_generation_multicore_bench {
    static auto find_number_of_zeroes(const std::string_view &address) -> size_t {
        size_t count = 0;
        for (auto i = 0; i < address.length(); ++i) {
            if (address[i] == '0') { count++; }
        }
        return count;
    }
    auto static generate_addresses(size_t num_addresses) -> void {
        for (size_t i = 0; i < num_addresses; ++i) {
            const auto entrop = walletpp::entropy::generate_entropy(32);
            const auto words = walletpp::bip39::mnemonic_from_entropy(entrop);
            const auto seed = walletpp::bip39::seed_from_mnemonic(words);
            const auto b32 = walletpp::bip32::from_seed(seed);

            const auto key_pair_address_0 = b32.derive_keypair_with_path("m/44'/60'/0'/0/0")->get_key_pair();
            const auto address_0 = walletpp::ethereum_utils::generate_ethereum_address(key_pair_address_0.private_key.key);
            const auto xp = key_pair_address_0.private_key.to_base58_string();

            if (find_number_of_zeroes(xp) >= 16) {
                std::string worr_stream;
                for (auto word: words) {
                    worr_stream += word;
                    worr_stream += " ";
                }
                std::cout << "Found address with 16 or more zeroes: " << worr_stream << std::endl;
            }
        }
    }
public:
    auto static benchmark() -> void {
        unsigned int cores = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        size_t addresses_per_thread = NUM_ADDRESSES / cores;

        for (unsigned int i = 0; i < cores; ++i) {
            threads.emplace_back(generate_addresses, addresses_per_thread);
        }

        for (auto& th : threads) {
            th.join();
        }
    }
};
