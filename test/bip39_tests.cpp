//
// Created by ariel on 3/9/24.
//

#include <catch2/catch_test_macros.hpp>
#include "entropy.h"
#include "bip39.h"

TEST_CASE("Bip39 seems fine", "[generateMnemonicFromEntropy]") {
    const auto random_entropy = entropy::generate_entropy(32);
    bip39::mnemonic_from_entropy(random_entropy);
    REQUIRE(random_entropy.size() == 32);
}
