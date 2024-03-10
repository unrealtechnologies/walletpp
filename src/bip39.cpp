//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "wordlist.h"

Botan::secure_vector<uint8_t> bip39::checksum_from_entropy(const Botan::secure_vector<uint8_t> &entropy) {
    const auto checksum_bits_length = entropy.size() / entropy_bits_multiple;
    const auto checksum_full = crypto_algorithms::sha256(entropy);
    return {checksum_full.begin(), checksum_full.begin() + checksum_bits_length};
}

Botan::secure_vector<uint16_t> bip39::words_index_from_entropy(const Botan::secure_vector<uint8_t> &entropy_with_checksum) {
    // we can use a bool to represent bits (0s and 1s) true is 1, false is 0
    Botan::secure_vector<bool> bits;
    for (const auto byte: entropy_with_checksum) {
        std::bitset<8> bitSet(byte);
        for (int i = 7; i >= 0; --i) {// Start from the most significant bit
            bits.push_back(bitSet.test(i) ? true : false);
        }
    }

    // Vector of vectors to hold the split vectors
    std::vector<Botan::secure_vector<uint8_t>> vector_of_word_bits;
    for (size_t i = 0; i < bits.size(); i += entropy_bits_per_word) {
        // Calculate the range for the current sub-vector
        const auto start_it = std::next(bits.begin(), i);
        const auto end_it = std::next(bits.begin(), std::min(bits.size(), i + entropy_bits_per_word));

        // Create and add the sub-vector to the vector of vectors
        Botan::secure_vector<uint8_t> subVector(start_it, end_it);
        vector_of_word_bits.push_back(subVector);
    }

    Botan::secure_vector<uint16_t> words_index;
    for (auto word_bits: vector_of_word_bits) {
        std::bitset<entropy_bits_per_word> bitset;
        // Copy the bits from the vector to the bitset
        for (size_t i = 0; i < word_bits.size(); ++i) {
            bitset[i] = word_bits[word_bits.size() - i - 1];// Reverse order to match LSB to MSB convention
        }
        words_index.emplace_back(static_cast<uint16_t>(bitset.to_ulong()));
    }

    return words_index;
}

std::vector<Botan::secure_vector<uint8_t>> bip39::words_from_words_index(const Botan::secure_vector<uint16_t> &words_index) {
    std::vector<Botan::secure_vector<uint8_t>> words;
    words.reserve(words_index.size());
    for (const auto word_index: words_index) {
        Botan::secure_vector<uint8_t> secure_vector;

        for (std::string_view word = english_word_list[word_index]; auto character: word) {
            secure_vector.emplace_back(character);
        }

        words.emplace_back(secure_vector);
    }
    return words;
}

std::vector<Botan::secure_vector<uint8_t>> bip39::mnemonic_from_entropy(const Botan::secure_vector<uint8_t> &entropy) {
    if (entropy.size() < entropy_min_length_in_bytes ||
        entropy.size() > entropy_max_length_in_bytes ||
        entropy.size() % sizeof(uint8_t) != 0) {
        throw std::runtime_error("Key size should be between 128 and 256 bits AKA 16 and 32 bytes");
    }

    const auto checksum = checksum_from_entropy(entropy);

    // combine the initial entropy with the checksum
    Botan::secure_vector<uint8_t> entropy_with_checksum;
    entropy_with_checksum.reserve(entropy.size() + checksum.size());
    entropy_with_checksum.insert(entropy_with_checksum.end(), entropy.begin(), entropy.end());
    entropy_with_checksum.insert(entropy_with_checksum.end(), checksum.begin(), checksum.end());

    const auto words_index = words_index_from_entropy(entropy_with_checksum);
    return words_from_words_index(words_index);
}
