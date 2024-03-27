#include <catch2/catch_test_macros.hpp>
#include "uncopyable_extended_key.h"

TEST_CASE("Tests run correctly", "[testRuns]") {

    // walletpp::secure_vector<uint8_t> vec1= {0x00, 0x01, 0x02, 0x03, 0x04};
    // walletpp::secure_vector<uint8_t> vec2= {0x00, 0x01, 0x02, 0x03, 0x04};
    // walletpp::secure_vector<uint8_t> vec3= {0x00, 0x01, 0x02, 0x03, 0x04};
    //
    // auto u_ext = walletpp::uncopyable_extended_key(std::move(vec1), std::move(vec2), std::move(vec3), 0, 0);
    //
    // std::cout << vec1.data() << std::endl;



    const std::string test;
    REQUIRE(test.empty());
}
