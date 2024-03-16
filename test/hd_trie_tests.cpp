//
// Created by Ariel Saldana on 3/14/24.
//
#include "catch2/catch_test_macros.hpp"
#include "crypto_algorithms.h"
#include "ethereum_utils.h"
#include "hd_trie.h"

SCENARIO("Deriving key-pair values given a bip32 tree path string", "[hd-trie]") {
    const std::string seed_hex_string = "000102030405060708090a0b0c0d0e0f";
    const auto seed = crypto_algorithms::from_hex(seed_hex_string);
    const auto trie = std::make_unique<hd_trie>(seed);

    WHEN("We generate the keypair for path: m") {
        auto key_pair = trie->search("m");
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi");
        }
    }

    WHEN("We generate the keypair for path: m/0") {
        auto key_pair = trie->search("m/0");
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9uHRZZhbkedL37eZEnyrNsQPFZYRAvjy5rt6M1nbEkLSo378x1CQQLo2xxBvREwiK6kqf7GRNvsNEchwibzXaV6i5GcsgyjBeRguXhKsi4R");
        }

        THEN("The public address for ethereum should be correct") {
            auto address = ethereum_utils::generate_ethereum_address(key_pair.private_key.key);
            REQUIRE(address == "0xAEfbb50942817d8270Bb9bD922aA5ca9cb06cDBf");
        }
    }

    WHEN("We generate the keypair for path: m/0/1") {
        auto key_pair = trie->search("m/0/1");
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9ww7sMFLzJMzy7bV1qs7nGBxgKYrgcm3HcJvGb4yvNhT9vxXC7eX7WVULzCfxucFEn2TsVvJw25hH9d4mchywguGQCZvRgsiRaTY1HCqN8G");
        }
    }

    WHEN("We generate the keypair for path: m/0/1/4/5") {
        auto key_pair = trie->search("m/0/1/4/5");
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprvA1rfzDvQx936D2ZdVpMfhPupVUBnzpkweP2F7dYM1g1ofTogN4TU4FdH1oUrmkPtc96itjUg3XfuHhqprdrMRmCwer9VeCaxGBWZKhSerWC");
        }
    }
}
