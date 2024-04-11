//
// Created by Ariel Saldana on 3/13/24.
//


#include "catch2/catch_test_macros.hpp"
#include "master_key_generator.h"

SCENARIO("We generate the private and public master keys", "[master-key-gen]") {
    const std::string seed_hex_string = "5EB00BBDDCF069084889A8AB9155568165F5C453CCB85E70811AAED6F6DA5FC19A5AC40B389CD370D086206DEC8AA6C43DAEA6690F20AD3D8D48B2D2CE9E38E4";
    const std::string expected_bae58_private_key_string = "xprv9s21ZrQH143K3GJpoapnV8SFfukcVBSfeCficPSGfubmSFDxo1kuHnLisriDvSnRRuL2Qrg5ggqHKNVpxR86QEC8w35uxmGoggxtQTPvfUu";
    const std::string expected_bae58_public_key_string = "xpub661MyMwAqRbcFkPHucMnrGNzDwb6teAX1RbKQmqtEF8kK3Z7LZ59qafCjB9eCRLiTVG3uxBxgKvRgbubRhqSKXnGGb1aoaqLrpMBDrVxga8";
    const auto seed = walletpp::crypto_algorithms::from_hex(seed_hex_string);
    const auto master_extended_key = walletpp::master_key_generator::generate_private_key(seed);
    const auto base58_key = master_extended_key->to_base58_string();
    const std::string generated_base58_string(base58_key.begin(), base58_key.end());

    GIVEN("We have the private key") {

        THEN("We validate the private key is correct") {
            REQUIRE(generated_base58_string == expected_bae58_private_key_string);
        }

        WHEN("We generate the public key from the private extended key") {
            const auto master_extended_public_key = walletpp::master_key_generator::generate_public_key(*master_extended_key);
            const auto base58_public_key = master_extended_public_key->to_base58_string();
            const std::string generated_base58_public_key_string(base58_public_key.begin(), base58_public_key.end());

            THEN("We validate the public key is correct") {
                REQUIRE(generated_base58_public_key_string == expected_bae58_public_key_string);
            }
        }
    }
}
