//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "crypto_algorithms.h"
#include "wordlist.h"

std::vector<uint8_t> bip39::checksum_from_entropy(const std::vector<uint8_t> &entropy) {
    const auto checksum_bits_length = entropy.size() / entropy_bits_multiple;
    const auto checksum_full = crypto_algorithms::sha256(entropy);
    return {checksum_full.begin(), checksum_full.begin() + checksum_bits_length};
}

std::vector<uint16_t> bip39::words_index_from_entropy(const std::vector<uint8_t> &entropy_with_checksum) {
    std::vector<bool> bits;
    for (const auto byte: entropy_with_checksum) {
        std::bitset<8> bitSet(byte);
        for (int i = 7; i >= 0; --i) {// Start from the most significant bit
            bits.push_back(bitSet.test(i) ? 1 : 0);
        }
    }

    // Vector of vectors to hold the split vectors
    std::vector<std::vector<uint8_t>> vector_of_word_bits;

    for (size_t i = 0; i < bits.size(); i += entropy_bits_per_word) {
        // Calculate the range for the current sub-vector
        const auto start_it = std::next(bits.begin(), i);
        const auto end_it = std::next(bits.begin(), std::min(bits.size(), i + entropy_bits_per_word));

        // Create and add the sub-vector to the vector of vectors
        std::vector<uint8_t> subVector(start_it, end_it);
        vector_of_word_bits.push_back(subVector);
    }

    std::vector<uint16_t> words_index;
    for (auto bits: vector_of_word_bits) {
        std::bitset<11> bitset;
        // Copy the bits from the vector to the bitset
        for (size_t i = 0; i < bits.size(); ++i) {
            bitset[i] = bits[bits.size() - i - 1];// Reverse order to match LSB to MSB convention
        }
        words_index.emplace_back(bitset.to_ulong());
    }

    return words_index;
}

std::vector<std::string> bip39::words_from_words_index(const std::vector<uint16_t> &words_index) {
    std::vector<std::string> words;
    for (const auto word_index: words_index) {
        words.emplace_back(english_word_list[word_index]);
    }
    return words;
}

std::vector<std::string> bip39::mnemonic_from_entropy(const std::vector<uint8_t> &entropy) {
    const auto checksum = checksum_from_entropy(entropy);

    std::vector<uint8_t> entropy_with_checksum;
    entropy_with_checksum.reserve(entropy.size() + checksum.size());
    entropy_with_checksum.insert(entropy_with_checksum.end(), entropy.begin(), entropy.end());
    entropy_with_checksum.insert(entropy_with_checksum.end(), checksum.begin(), checksum.end());

    auto words_index = words_index_from_entropy(entropy_with_checksum);
    return words_from_words_index(words_index);
}
