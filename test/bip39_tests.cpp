//
// Created by ariel on 3/9/24.
//

#include "bip39.h"
#include "crypto_algorithms.h"
#include "entropy.h"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <botan/hex.h>

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

    GIVEN("We generate the seed from entropy") {
        WHEN("We use a 16 byte entropy string") {
            const std::string entropy_hex_str = "00000000000000000000000000000000";
            const auto entropy = crypto_algorithms::from_hex(entropy_hex_str);
            const auto words = bip39::mnemonic_from_entropy(entropy);
            const auto checksum_word = words[words.size() - 1];
            const auto seed_bytes = bip39::seed_from_mnemonic(words);
            const auto seed_hex = Botan::hex_encode(seed_bytes);
            const auto correct_seed_hex = "5EB00BBDDCF069084889A8AB9155568165F5C453CCB85E70811AAED6F6DA5FC19A5AC40B389CD370D086206DEC8AA6C43DAEA6690F20AD3D8D48B2D2CE9E38E4";

            THEN("The seed hex value should be correct") {
                REQUIRE(seed_hex == correct_seed_hex);
            }
            THEN("The seed hex value should be the correct size") {
                const auto correct_seed_hex_size = std::strlen(correct_seed_hex);
                REQUIRE(seed_hex.length() == correct_seed_hex_size);
            }
        }
    }
}
