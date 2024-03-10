//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "wordlist.h"

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

    // Vector of vectors to hold the split vectors
    std::vector<Botan::secure_vector<uint8_t>> vector_of_word_bits;
    for (size_t i = 0; i < entropy_with_checksum.size(); i += entropy_bits_per_word) {
        // Calculate the range for the current sub-vector
        const auto start_it = std::next(entropy_with_checksum.begin(), i);
        const auto end_it = std::next(entropy_with_checksum.begin(), std::min(entropy_with_checksum.size(), i + entropy_bits_per_word));

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

    return words_from_words_index(words_index);
}

std::array<uint8_t, crypto_algorithms::pbkdf2_sha512_output_byte_size> bip39::seed_from_mnemonic(const std::vector<Botan::secure_vector<uint8_t>> &words_vector) {
    Botan::secure_vector<uint8_t> seed{};
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

    return crypto_algorithms::pbkdf2(result, "mnemonic");
}
