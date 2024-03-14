//
// Created by Ariel Saldana on 3/14/24.
//

#include "catch2/catch_test_macros.hpp"
#include "crypto_algorithms.h"
#include "hd_derivation.h"
#include "master_key_generator.h"

SCENARIO("With a master private key, we derive child keys", "[hd-derivation]") {
    const std::string seed_hex_string = "000102030405060708090a0b0c0d0e0f";
    const auto seed = crypto_algorithms::from_hex(seed_hex_string);
    const auto master_extended_key = walletpp::master_key_generator::generate_private_key(seed);

    GIVEN("We have the master extended private key") {


        THEN("We derive the private child key") {
            auto child_key = hd_derivation::private_child_key_derivation(*master_extended_key, 0);
            const auto base58_key = child_key.to_base58();
            const std::string generated_base58_string(base58_key.begin(), base58_key.end());
            REQUIRE(generated_base58_string == "xprv9uHRZZhbkedL37eZEnyrNsQPFZYRAvjy5rt6M1nbEkLSo378x1CQQLo2xxBvREwiK6kqf7GRNvsNEchwibzXaV6i5GcsgyjBeRguXhKsi4R");
        }

        // WHEN("We generate the public key from the private extended key") {
        //     // const auto base58_public_key = master_extended_public_key->to_base58();
        //     // const std::string generated_base58_public_key_string(base58_public_key.begin(), base58_public_key.end());
        //
        //     THEN("We validate the public key is correct") {
        //         // REQUIRE(generated_base58_public_key_string == expected_bae58_public_key_string);
        //     }
        // }
    }
}
