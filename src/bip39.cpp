//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "crypto_algorithms.h"

std::vector<uint8_t> bip39::checksum_from_entropy(const std::vector<uint8_t> &entropy) {
    auto checksum_bits_length = entropy.size() / entropy_bits_multiple;
    auto checksum_full = crypto_algorithms::sha256(entropy);
    std::vector<uint8_t> checksum(checksum_full.begin(), checksum_full.begin() + checksum_bits_length);
    return checksum;
}


std::vector<std::string> bip39::mnemonic_from_entropy(const std::vector<uint8_t> &entropy) {
    auto checksum = checksum_from_entropy(entropy);
    std::vector<uint8_t> entropy_with_checksum;
    entropy_with_checksum.reserve(entropy.size() + checksum.size());
    entropy_with_checksum.insert(entropy_with_checksum.end(), entropy.begin(), entropy.end());
    entropy_with_checksum.insert(entropy_with_checksum.end(), checksum.begin(), checksum.end());

    int t = entropy_with_checksum.size() / 11;

    auto vec = std::vector<std::string>();
    return vec;
}
