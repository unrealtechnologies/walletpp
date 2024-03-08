//
// Created by ariel on 3/8/24.
//
#include <catch2/catch_test_macros.hpp>
#include "entropy.h"

TEST_CASE("Entropy generates correctly", "[generateEntropy]") {
    const auto random_entropy = entropy::generate_entropy(256);
    REQUIRE(random_entropy.size() == 256);
}