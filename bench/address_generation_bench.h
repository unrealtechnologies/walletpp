//
// Created by Ariel Saldana on 3/28/24.
//

//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "entropy.h"
#include "ethereum_utils.h"

#include <chrono>
#include <thread>
#include <vector>

class address_generation_bench {

public:
    static auto find_number_of_zeroes(const std::string_view &address) -> size_t {
        size_t count = 0;
        for (auto i = 0; i < address.length(); ++i) {
            if (address[i] == '0') { count++; }
        }

        return count;
    }

    static void benchmark() {
        constexpr size_t NUM_ADDRESSES = 5000;

        for (size_t i = 0; i < NUM_ADDRESSES; ++i) {
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
};
