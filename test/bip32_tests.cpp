//
// Created by Ariel Saldana on 3/12/24.
//

#include "bip32.h"
#include "catch2/catch_test_macros.hpp"
#include "hd_derivation.h"


#include <botan/hash.h>
#include <botan/secmem.h>
#include <string>

SCENARIO("BIP32 generates master key") {
    GIVEN("A seed hex string produced from bip39") {
        // std::string seed_hex_string = "5EB00BBDDCF069084889A8AB9155568165F5C453CCB85E70811AAED6F6DA5FC19A5AC40B389CD370D086206DEC8AA6C43DAEA6690F20AD3D8D48B2D2CE9E38E4";
        // auto seed_vector = Botan::hex_decode(seed_hex_string);
        // Botan::secure_vector<uint8_t> seed{ seed_vector.begin(), seed_vector.end()};
        // // std::copy_n(std::make_move_iterator(seed_vector.begin()), 32, seed.begin());
        // auto master_extended_key = bip32::generate_master_key(seed);
        // auto base58_key = master_extended_key->to_base58();
        // std::string generated_base58_string(base58_key.begin(), base58_key.end());
        // REQUIRE(generated_base58_string == "xprv9s21ZrQH143K3GJpoapnV8SFfukcVBSfeCficPSGfubmSFDxo1kuHnLisriDvSnRRuL2Qrg5ggqHKNVpxR86QEC8w35uxmGoggxtQTPvfUu");

        // auto public_key = crypto_algorithms::generate_public_key(master_extended_key->key, true);
        // auto master_extended_public_key = std::make_unique<extended_key>(public_key, master_extended_key->chain_code, master_extended_key->parent_finger_print, 0, 0);
        // auto base58_public_key = master_extended_key->to_base58();
        // std::string generated_base58_public_string(base58_public_key.begin(), base58_public_key.end());
        // REQUIRE(generated_base58_public_string == "xpub661MyMwAqRbcFkPHucMnrGNzDwb6teAX1RbKQmqtEF8kK3Z7LZ59qafCjB9eCRLiTVG3uxBxgKvRgbubRhqSKXnGGb1aoaqLrpMBDrVxga8");

    }
}
