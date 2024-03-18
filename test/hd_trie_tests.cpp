//
// Created by Ariel Saldana on 3/14/24.
//
#include "catch2/catch_test_macros.hpp"
#include "crypto_algorithms.h"
#include "ethereum_utils.h"
#include "hd_derivation.h"
#include "hd_trie.h"

SCENARIO("Deriving key-pair values given a bip32 tree path string", "[hd-trie]") {
    const std::string seed_hex_string = "000102030405060708090a0b0c0d0e0f";
    const auto seed = crypto_algorithms::from_hex(seed_hex_string);
    const auto trie = std::make_unique<hd_trie>(seed);

    WHEN("We generate the keypair for path: m") {
        auto key_pair = trie->search("m")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi");
        }
    }

    WHEN("We generate the keypair for path: m/0") {
        auto key_pair = trie->search("m/0")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9uHRZZhbkedL37eZEnyrNsQPFZYRAvjy5rt6M1nbEkLSo378x1CQQLo2xxBvREwiK6kqf7GRNvsNEchwibzXaV6i5GcsgyjBeRguXhKsi4R");
        }

        THEN("The public address for ethereum should be correct") {
            auto address = ethereum_utils::generate_ethereum_address(key_pair.private_key.key);
            REQUIRE(address == "0xAEfbb50942817d8270Bb9bD922aA5ca9cb06cDBf");
        }
    }

    WHEN("We generate the keypair for path: m/0/1") {
        auto key_pair = trie->search("m/0/1")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9ww7sMFLzJMzy7bV1qs7nGBxgKYrgcm3HcJvGb4yvNhT9vxXC7eX7WVULzCfxucFEn2TsVvJw25hH9d4mchywguGQCZvRgsiRaTY1HCqN8G");
        }
    }

    WHEN("We generate the keypair for path: m/0/1/4/5") {
        auto key_pair = trie->search("m/0/1/4/5")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprvA1rfzDvQx936D2ZdVpMfhPupVUBnzpkweP2F7dYM1g1ofTogN4TU4FdH1oUrmkPtc96itjUg3XfuHhqprdrMRmCwer9VeCaxGBWZKhSerWC");
        }
    }

    WHEN("We generate the keypair for path: m/0'") {
        auto key_pair = trie->search("m/0'")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7");
            REQUIRE(key_pair.public_key.to_base58_string() == "xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw");
        }
    }

    WHEN("We generate the keypair for path: m/0'/0") {
        auto key_pair = trie->search("m/0'/0")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9wTYmMFdV23N21MM6dLNavSQV7Sj7meSPXx6AV5eTdqqGLjycVjb115Ec5LgRAXscPZgy5G4jQ9csyyZLN3PZLxoM1h3BoPuEJzsgeypdKj");
            REQUIRE(key_pair.public_key.to_base58_string() == "xpub6ASuArnXKPbfEVRpCesNx4P939HDXENHkksgxsVG1yNp9958A33qYoPiTN9QrJmWFa2jNLdK84bWmyqTSPGtApP8P7nHUYwxHPhqmzUyeFG");
        }

        THEN("The public address for ethereum should be correct") {
            auto key_pair = trie->search("m/0'/0")->get_key_pair();
            auto address = ethereum_utils::generate_ethereum_address(key_pair.private_key.key);
            REQUIRE(address == "0x91860ef4Fc12f4dCa2564A3f7fcceA9325831aC6");
        }
    }

    WHEN("We generate the keypair for path: m/44'/60'/0'/0") {
        auto key_pair = trie->search("m/44'/60'/0'/0")->get_key_pair();
        THEN("Private and Public key should be correct") {
            REQUIRE(key_pair.private_key.to_base58_string() == "xprv9zZhZKG7taxeit8w1HiTDdUko2Fm1RxkrjxANbEaG7kFvJp5UEh6MiQ5b5XvwWg8xdHMhueagettVG2AbfqSRDyNpxRDBLyMSbNq1KhZ8ai");
            REQUIRE(key_pair.public_key.to_base58_string() == "xpub6DZ3xpo1ixWwwNDQ7KFTamRVM46FQtgcDxsmAyeBpTHEo79E1n1LuWiZSMSRhqMQmrHaqJpek2TbtTzbAdNWJm9AhGdv7iJUpDjA6oJD84b");
        }

        THEN("We're able to derive the first 10 unhardened child keys with index 1..10 and verify ethereum addresses") {
            auto child_index_0 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 0);
            auto child_index_1 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 1);
            auto child_index_2 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 2);
            auto child_index_3 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 3);
            auto child_index_4 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 4);
            auto child_index_5 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 5);
            auto child_index_6 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 6);
            auto child_index_7 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 7);
            auto child_index_8 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 8);
            auto child_index_9 = hd_derivation::private_and_public_key_pair_derivation(key_pair.private_key, 9);

            auto child_address_0 = ethereum_utils::generate_ethereum_address(child_index_0.private_key.key);
            auto child_address_1 = ethereum_utils::generate_ethereum_address(child_index_1.private_key.key);
            auto child_address_2 = ethereum_utils::generate_ethereum_address(child_index_2.private_key.key);
            auto child_address_3 = ethereum_utils::generate_ethereum_address(child_index_3.private_key.key);
            auto child_address_4 = ethereum_utils::generate_ethereum_address(child_index_4.private_key.key);
            auto child_address_5 = ethereum_utils::generate_ethereum_address(child_index_5.private_key.key);
            auto child_address_6 = ethereum_utils::generate_ethereum_address(child_index_6.private_key.key);
            auto child_address_7 = ethereum_utils::generate_ethereum_address(child_index_7.private_key.key);
            auto child_address_8 = ethereum_utils::generate_ethereum_address(child_index_8.private_key.key);
            auto child_address_9 = ethereum_utils::generate_ethereum_address(child_index_9.private_key.key);

            REQUIRE(child_address_0 == "0x022b971dFF0C43305e691DEd7a14367AF19D6407");
            REQUIRE(child_address_1 == "0xbb7A182240010703dc81D6b1EFf630CA02a169FD");
            REQUIRE(child_address_2 == "0xECf722a6a8EE18F5A9D3C00D168be3D0d068732b");
            REQUIRE(child_address_3 == "0x23FcfBa6579ABdCf799c65fE87e7b2668Eb78Ed8");
            REQUIRE(child_address_4 == "0x1FB339d4F55343e5dfE2a848bCC41440C158a9d0");
            REQUIRE(child_address_5 == "0xa9b34532Dccb4F308bE78A753b50907564A88bC7");
            REQUIRE(child_address_6 == "0x0bdD08f4E768d153a7eBF3648717df141c3F9ba9");
            REQUIRE(child_address_7 == "0xEC277d3fc7F1639dC801070dD075f6761Ac3804d");
            REQUIRE(child_address_8 == "0xc6f128BCd60239aAB96eAE74821cc26CF9b10984");
            REQUIRE(child_address_9 == "0xF0f1198971c23091F6b3C9Fab3098dD0EBD010a7");
        }
    }
}
