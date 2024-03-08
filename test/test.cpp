#include <catch2/catch_test_macros.hpp>

TEST_CASE("Tests run correctly", "[testRuns]") {
    const std::string test;
    REQUIRE(test.empty());
}