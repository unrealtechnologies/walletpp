//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "crypto_algorithms.h"
#include "entropy.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Bip39 seems fine", "[generateMnemonicFromEntropy]") {
    const auto random_entropy = entropy::generate_entropy(32);
    REQUIRE(random_entropy.size() == 32);
}

SCENARIO("BIP39 Generates mnemonic words", "[bip39-checksum]") {
    GIVEN("We generate mnemonic words from entropy") {
        WHEN("We use a 16 byte entropy string") {
            const std::string entropy_hex_str = "00000000000000000000000000000000";
            const auto entropy = crypto_algorithms::from_hex(entropy_hex_str);
            const auto words = bip39::mnemonic_from_entropy(entropy);
            const auto checksum_word = words[words.size() - 1];
            THEN("The list of words size should be 12") {
                REQUIRE(words.size() == 12);
            }
            THEN("The checksum is correct") {
                REQUIRE(checksum_word == "about");
            }
        }

        WHEN("We use a 32 byte entropy string") {
            const std::string entropy_hex_str = "0000000000000000000000000000000000000000000000000000000000000000";
            const auto entropy = crypto_algorithms::from_hex(entropy_hex_str);
            const auto words = bip39::mnemonic_from_entropy(entropy);
            const auto checksum_word = words[words.size() - 1];
            THEN("The list of words size should be 24") {
                REQUIRE(words.size() == 24);
            }
            THEN("The checksum is correct") {
                REQUIRE(checksum_word == "art");
            }
        }
    }
}
