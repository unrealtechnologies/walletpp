//
// Created by Ariel Saldana on 3/16/24.
//

#include "bip32.h"
#include "bip39.h"
#include "bip44.h"
#include "decrypt_file.h"
#include "encrypt_file.h"
#include "entropy.h"
#include "ethereum_utils.h"
#include "wallet_information.h"

#include <format>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

void encrypt_json_string(const std::string &public_key_file_path, const std::string &file_prefix, const std::string &msg) {
    unsigned char key[AES_256_KEY_SIZE];
    unsigned char iv[AES_GCM_IV_SIZE];
    std::vector<unsigned char> tag;
    std::vector<unsigned char> ciphertext = encrypt(public_key_file_path, file_prefix, msg, key, iv, tag);

    std::ofstream encfile(file_prefix + "_encrypted.bin", std::ios::binary);
    encfile.write(reinterpret_cast<const char *>(iv), AES_GCM_IV_SIZE);
    encfile.write(reinterpret_cast<const char *>(tag.data()), AES_GCM_TAG_SIZE);
    encfile.write(reinterpret_cast<const char *>(ciphertext.data()), ciphertext.size());
}

void decrypt_json_string(const std::string &private_key_file) {
    // Read the encrypted AES key
    std::vector<unsigned char> encrypted_key = read_file("encrypted_key.bin");

    // Decrypt the AES key using the private key
    std::vector<unsigned char> key = decrypt_aes_key(private_key_file, encrypted_key);

    // Read the IV, tag, and ciphertext
    std::vector<unsigned char> iv(AES_GCM_IV_SIZE);
    std::vector<unsigned char> tag(AES_GCM_TAG_SIZE);
    std::vector<unsigned char> ciphertext;

    std::ifstream encfile("encrypted.bin", std::ios::binary);
    encfile.read(reinterpret_cast<char *>(iv.data()), AES_GCM_IV_SIZE);
    encfile.read(reinterpret_cast<char *>(tag.data()), AES_GCM_TAG_SIZE);
    ciphertext.assign(std::istreambuf_iterator<char>(encfile), std::istreambuf_iterator<char>());

    // Decrypt the ciphertext
    std::string plaintext = decrypt(ciphertext, key.data(), iv.data(), tag);

    std::cout << "Decrypted text: " << plaintext << std::endl;
}

std::string generate_random_number() {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99999999);

    // Generate random number
    int random_number = dis(gen);

    // Convert to zero-padded string
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << random_number;
    return oss.str();
}

auto log(const std::string &msg) -> void {
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y/%m/%d %I:%M%p") << " application: " << msg;
    const std::string log_msg = ss.str();

    // Now log_msg contains the date, time and the message
    // You can print it to console or write it to a file
    std::cout << log_msg << std::endl;
}

auto format_file_name(const std::string_view &file, unsigned int index) -> std::string {
    std::string result;

    if (file.size() < 2) { throw std::runtime_error("that's not a correct string format"); }

    for (auto i = 0; i < file.size(); i++) {
        constexpr char r = '}';
        constexpr char l = '{';
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


void find_address(unsigned int start, unsigned int step, const std::string &file, const std::string &public_key_path) {
    while (true) {
        std::string mnemonic_passphrase = generate_random_number();
        const auto ent = walletpp::entropy::generate_entropy(32);
        const auto words = walletpp::bip39::mnemonic_from_entropy(ent);
        const auto seed = walletpp::bip39::seed_from_mnemonic(words, mnemonic_passphrase);
        const auto b44 = walletpp::bip44::generate(walletpp::coins::Ethereum, 0);
        const auto b32 = walletpp::bip32::from_seed(seed);
        const auto key_pair = b32.derive_keypair_with_path(b44.to_path_string())->get_key_pair();

        const auto address = walletpp::ethereum_utils::generate_ethereum_address(key_pair.first.key);
        const auto address_without_prefix = address.substr(2, 42);

        constexpr int repeated_string_length = 4;
        constexpr int number_of_zeroes = 8;
        if (find_leading_zeroes(address_without_prefix, number_of_zeroes) || find_repeated_characters_start_end(address_without_prefix, '0', repeated_string_length) ||
            find_number_of_zeroes(address_without_prefix) >= 16) {

            std::string word_stream;
            for (auto word: words) {
                word_stream += word;
                word_stream += " ";
            }
            word_stream = word_stream.substr(0, word_stream.length() - 1);

            std::string bip39_seed = walletpp::crypto_algorithms::to_hex(seed);
            std::string address_private_key = "0x" + walletpp::crypto_algorithms::to_hex(key_pair.first.key);
            std::string address_public_key = "0x" + walletpp::crypto_algorithms::to_hex(key_pair.second.key);
            std::string address_derivation_path = b44.to_path_string();
            auto wallet_info = wallet_information(address, address_private_key, address_public_key, word_stream, mnemonic_passphrase, bip39_seed);
            std::string json_string = wallet_info.to_json_string();

            // encrypt json string
            encrypt_json_string(public_key_path, address, json_string);

            // log the newly found address
            log("Found address: " + address);
        }
    }
}

int main(int argc, char *argv[]) {
    const char *public_key_path = nullptr;
    static struct option long_options[] = {{"public_key", required_argument, nullptr, 'p'}, {nullptr, 0, nullptr, 0}};

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "p:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'p':
                public_key_path = optarg;
                break;
            case '?':
                // getopt_long already prints an error message.
                break;
            default:
                abort();
        }
    }

    if (public_key_path) {
        std::cout << "Public key path: " << public_key_path << std::endl;
    } else {
        std::cerr << "Public key path not provided." << std::endl;
        return 69;
    }

    log("Starting address generation");
    const std::string log_file_format = "./addresses-rare-{}.txt";

    unsigned int cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (unsigned int i = 0; i < cores; ++i) { threads.emplace_back(find_address, i, cores, log_file_format, std::string(public_key_path)); }
    for (auto &th: threads) { th.join(); }

    return 0;
}
