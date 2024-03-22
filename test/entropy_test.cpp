//
// Created by ariel on 3/8/24.
//
#include "bip39.h"


#include "entropy.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Entropy generates correctly", "[generateEntropy]") {
    const auto random_entropy = walletpp::entropy::generate_entropy(256);
    REQUIRE(random_entropy.size() == 256);
}
