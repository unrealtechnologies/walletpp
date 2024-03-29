//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "bip39.h"
#include "bip44.h"
#include "entropy.h"
#include "ethereum_utils.h"
#include <fstream>
#include <thread>
#include <vector>

auto format_file_name(const std::string_view &file, unsigned int index) -> std::string {
    char l = '{';
    char r = '}';
    std::string result;

    if (file.size() < 2) { throw std::runtime_error("that's not a correcr string format"); }

    for (auto i = 0; i < file.size(); i++) {
        if (file[i] == r && file[i - 1] == l) {
            result += std::to_string(index);
            continue;
        }

        if (file[i] == l || file[i] == r) { continue; }

        result += file[i];
    }

    return result;
}

bool find_repeated_characters_start_end(const std::string_view &address, const char &character, const size_t &repeated_count) {
    return address.substr(0, repeated_count).find_first_not_of(character) == std::string::npos &&
           address.substr(address.size() - repeated_count, repeated_count).find_first_not_of(character) == std::string::npos;
}

bool find_any_repeated_characters_start_end(const std::string_view &address, const size_t &repeated_count) {
    const auto starting_character = address[0];
    if (starting_character != address[address.length() - 1]) { return false; }
    for (auto i = 1; i < repeated_count; ++i) {
        if (address[i] != starting_character) { return false; }

        if (address[address.length() - 1 - i] != starting_character) { return false; }
    }

    return true;
}

bool find_leading_zeroes(const std::string_view &address, const size_t &number_of_zeroes) {
    return address.substr(0, number_of_zeroes).find_first_not_of('0') == std::string::npos;
}

auto find_number_of_zeroes(const std::string_view &address) -> size_t {
    size_t count = 0;
    for (auto i = 0; i < address.length(); ++i) {
        if (address[i] == '0') { count++; }
    }

    return count;
}

void find_address(unsigned int start, unsigned int step, const std::string &file) {
    auto fstring = format_file_name(file, start);

    while (true) {
        const auto ent = walletpp::entropy::generate_entropy(32);
        const auto words = walletpp::bip39::mnemonic_from_entropy(ent);
        const auto seed = walletpp::bip39::seed_from_mnemonic(words);
        const auto b44 = walletpp::bip44::generate(walletpp::coins::Ethereum, 0);
        const auto b32 = walletpp::bip32::from_seed(seed);
        const auto key_pair = b32.derive_keypair_with_path(b44.to_path_string())->get_key_pair();

        const auto address = walletpp::ethereum_utils::generate_ethereum_address(key_pair.private_key.key);
        const auto address_without_prefix = address.substr(2, 42);

        constexpr int repeated_string_length = 4;
        constexpr int number_of_zeroes = 8;
        if (find_leading_zeroes(address_without_prefix, number_of_zeroes) || find_repeated_characters_start_end(address_without_prefix, '0', repeated_string_length) ||
            find_number_of_zeroes(address_without_prefix) >= 16) {

            std::string worr_stream;
            for (auto word: words) {
                worr_stream += word;
                worr_stream += " ";
            }

            std::ofstream outfile;
            outfile.open(format_file_name(file, start), std::ios_base::app);
            outfile << key_pair.private_key.to_base58_string() << std::endl;
            outfile << walletpp::crypto_algorithms::to_hex(key_pair.private_key.key) << std::endl;
            outfile << worr_stream << std::endl;
            outfile << address << std::endl;
            outfile << std::endl;
            outfile.close();
        }
    }
}

int main() {
    const std::string log_file_format = "./addresses-rare-{}.txt";

    unsigned int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < cores; ++i) { threads.emplace_back(find_address, i, cores, log_file_format); }

    for (auto &th: threads) { th.join(); }

    return 0;
}
