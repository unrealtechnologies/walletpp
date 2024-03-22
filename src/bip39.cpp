//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "wordlist.h"

namespace walletpp {
    Botan::secure_vector<uint8_t> bip39::checksum_from_entropy(const Botan::secure_vector<uint8_t> &entropy) {
        const auto checksum_full = crypto_algorithms::sha256(entropy);
        return {checksum_full.begin(), checksum_full.end()};
    }

    Botan::secure_vector<bool> bip39::checksum_bits_from_entropy(const Botan::secure_vector<uint8_t> &entropy) {
        size_t checksum_bits_length = entropy.size() * single_byte_bits_length / entropy_bits_multiple;
        auto bits = crypto_algorithms::binary_from_bytes(entropy, checksum_bits_length);
        return bits;
    }

    Botan::secure_vector<uint16_t> bip39::words_index_from_entropy(const Botan::secure_vector<bool> &entropy_with_checksum) {
        if (entropy_with_checksum.size() % entropy_bits_per_word != 0) {
            throw std::runtime_error("The number of bits is not a multiple of the bits per word.");
        }

        const size_t num_words = entropy_with_checksum.size() / entropy_bits_per_word;
        Botan::secure_vector<uint16_t> words_index;
        words_index.reserve(num_words);

        for (size_t i = 0; i < num_words; ++i) {
            uint16_t word_index = 0;
            for (size_t j = 0; j < entropy_bits_per_word; ++j) {
                word_index <<= 1;// Shift left to make room for the next bit
                if (entropy_with_checksum[i * entropy_bits_per_word + j]) {
                    word_index |= 1;// Set the last bit if the corresponding bit in entropy_with_checksum is true
                }
            }
            words_index.emplace_back(word_index);
        }

        return words_index;
    }

    std::vector<std::string_view> bip39::mnemonic_from_entropy(const Botan::secure_vector<uint8_t> &entropy) {
        if (entropy.size() < entropy_min_length_in_bytes ||
            entropy.size() > entropy_max_length_in_bytes ||
            entropy.size() % sizeof(uint8_t) != 0) {
            throw std::runtime_error("Key size should be between 128 and 256 bits AKA 16 and 32 bytes");
        }

        size_t checksum_bits_length = entropy.size() * single_byte_bits_length / entropy_bits_multiple;
        const auto checksum = checksum_from_entropy(entropy);
        const auto entropy_bits = crypto_algorithms::binary_from_bytes(entropy, std::nullopt);
        const auto checksum_bits = crypto_algorithms::binary_from_bytes(checksum, checksum_bits_length);

        // combine the initial entropy with the checksum
        Botan::secure_vector<bool> entropy_with_checksum_bits;
        entropy_with_checksum_bits.reserve(entropy.size() + checksum.size());
        entropy_with_checksum_bits.insert(entropy_with_checksum_bits.end(), entropy_bits.begin(), entropy_bits.end());
        entropy_with_checksum_bits.insert(entropy_with_checksum_bits.end(), checksum_bits.begin(), checksum_bits.end());

        const auto words_index = words_index_from_entropy(entropy_with_checksum_bits);

        std::vector<std::string_view> words;
        words.reserve(words_index.size());
        for (const auto word_index: words_index) {
            std::string_view word = english_word_list[word_index];
            words.emplace_back(word);
        }

        return words;
    }

    std::array<uint8_t, pbkdf2_sha512_output_byte_size> bip39::seed_from_mnemonic(const std::vector<std::string_view> &words_vector, const std::string_view salt, const size_t number_of_pbkdf2_iterations) {
        std::string result;
        for (auto it = words_vector.begin(); it != words_vector.end(); ++it) {
            std::string word_as_string(it->begin(), it->end());
            result += word_as_string;

            // If not the last element, add a space
            if (std::next(it) != words_vector.end()) {
                result += " ";
            }
            crypto_algorithms::secure_erase_string(word_as_string);
        }

        const auto seed = crypto_algorithms::fast_pbkdf2(result, salt, number_of_pbkdf2_iterations);
        crypto_algorithms::secure_erase_string(result);

        return seed;
    }
}
