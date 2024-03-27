//
// Created by Ariel Saldana on 3/14/24.
//

#include "catch2/catch_test_macros.hpp"
#include "crypto_algorithms.h"
#include "hd_derivation.h"
#include "master_key_generator.h"

SCENARIO("Deriving first and secon level child keys from a master private key", "[hd-derivation]") {
    GIVEN("A valid seed for a master private key") {
        const std::string seed_hex_string = "000102030405060708090a0b0c0d0e0f";
        const auto seed = walletpp::crypto_algorithms::from_hex(seed_hex_string);
        const auto master_extended_key = walletpp::master_key_generator::generate_private_key(seed);

        auto keys_pair = walletpp::hd_derivation::private_and_public_key_pair_derivation(master_extended_key, 0);
        WHEN("We derive the first level child private and public keys from the master key") {

            auto private_key = keys_pair.private_key;
            auto public_key = keys_pair.public_key;
            auto base58_private_key_string = private_key.to_base58_string();
            auto base58_public_key_string = public_key.to_base58_string();

            THEN("The derived keys match the expected base58-encoded values") {
                REQUIRE(base58_private_key_string == "xprv9uHRZZhbkedL37eZEnyrNsQPFZYRAvjy5rt6M1nbEkLSo378x1CQQLo2xxBvREwiK6kqf7GRNvsNEchwibzXaV6i5GcsgyjBeRguXhKsi4R");
                REQUIRE(base58_public_key_string == "xpub68Gmy5EVb2BdFbj2LpWrk1M7obNuaPTpT5oh9QCCo5sRfqSHVYWex97WpDZzszdzHzxXDAzPLVSwybe4uPYkSk4G3gnrPqqkV9RyNzAcNJ1");
            }
        }

        AND_WHEN("We derive second level keys from the first level derived private key") {
            auto second_level_keys_pair = walletpp::hd_derivation::private_and_public_key_pair_derivation(keys_pair.private_key, 0);
            auto second_level_private_key = second_level_keys_pair.private_key;
            auto second_level_public_key = second_level_keys_pair.public_key;
            const auto base58_second_level_private_key = second_level_private_key.to_base58_string();
            const auto base58_second_level_public_key = second_level_public_key.to_base58_string();

            THEN("The derived second level keys match the expected base58-encoded values") {
                REQUIRE(base58_second_level_private_key == "xprv9ww7sMFLzJMzur2oEQDB642fbsMS4q6JRraMVTrM9bTWBq7NDS8ZpmsKVB4YF3mZecqax1fjnsPF19xnsJNfRp4RSyexacULXMKowSACTRc");
                REQUIRE(base58_second_level_public_key == "xpub6AvUGrnEpfvJ8L7GLRkBTByQ9uBvUHp9o5VxHrFxhvzV4dSWkySpNaBoLR9FpbnwRmTa69yLHF3QfcaxbWT7gWdwws5k4dpmJvqpEuMWwnj");
            }
        }
    }
}

