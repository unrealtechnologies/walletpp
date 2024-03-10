//
// Created by ariel on 3/9/24.
//

#include <catch2/catch_test_macros.hpp>
#include "entropy.h"
#include "bip39.h"
#include "crypto_algorithms.h"
#include <string>

TEST_CASE("Bip39 seems fine", "[generateMnemonicFromEntropy]") {
    const auto random_entropy = entropy::generate_entropy(32);
    REQUIRE(random_entropy.size() == 32);
}

SCENARIO("BIP39 Generates Correct Values", "[bip39]") {
    GIVEN("We have an entropy string") {
        const std::string entropy_hex_str = "00000000000000000000000000000000";
        const auto entropy = crypto_algorithms::from_hex(entropy_hex_str);
        const auto words = bip39::mnemonic_from_entropy(entropy);
        auto checksum_word = std::string(words[words.size() - 1].begin(), words[words.size() - 1].end());
        REQUIRE( checksum_word == "about");
        REQUIRE(words.size() == 12);
    }
}


