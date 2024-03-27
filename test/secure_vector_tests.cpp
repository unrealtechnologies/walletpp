//
// Created by Ariel Saldana on 3/26/24.
//


#include "secure_vector.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>

SCENARIO("We want to test the secure_vector class", "[secure-vector]") {
    GIVEN("We store a key in a secure vector") {
        THEN("When we check the key size, it should be 9 bytes long") {
            std::string private_key_string = "123456789";
            auto private_key = walletpp::secure_vector<uint8_t>(private_key_string.begin(), private_key_string.end());
            REQUIRE(private_key.size() == 9);
        }

        THEN("When we access the vector data at index, we should see the correct value") {
            std::string private_key_string = "123456789";
            auto private_key = walletpp::secure_vector<uint8_t>(private_key_string.begin(), private_key_string.end());
            REQUIRE(private_key.data()[5] == '6');
        }

        THEN("When we create a secure vector using initializer list, we should see the correct value") {
            auto private_key = walletpp::secure_vector<uint8_t>{0x01, 0x02, 0x03, 0x04, 0x05};
            REQUIRE(private_key.data()[1] == 0x02);
        }

        THEN("We should have access to .begin() and .end()") {
            auto private_key = walletpp::secure_vector<uint8_t>{0x01, 0x02, 0x03, 0x04, 0x05};
            std::vector<uint8_t> copy = {private_key.begin(), private_key.end()};
            REQUIRE(copy[1] == 0x02);
        }

        THEN("When we go out of scope, the key should be erased") {
            const std::string private_key_string = "123456789";
            walletpp::secure_vector<uint8_t> private_key{private_key_string.begin(), private_key_string.end()};

            auto dat = private_key.data();
            // Simulate destruction
            private_key.simulate_destruction();

            REQUIRE(dat[0] == 0);
        }

        THEN("Test what kyran says") {
            walletpp::secure_vector<std::string> vec{"Bacon ipsum dolor amet leberkas shankle cupim, tail short loin strip steak t-bone pork tongue shank. Ham leberkas flank beef ribs pancetta tenderloin chislic pork frankfurter salami pork loin bacon beef. Pancetta brisket ball tip, ham hock beef ribs t-bone venison short ribs shankle porchetta shank kielbasa.", "bar", "baz"};
            char *p = vec[0].data();

            auto dat = vec.data();
            // Simulate destruction
            vec.simulate_destruction();

            REQUIRE(dat[0] == "");
            REQUIRE(std::strlen(p) == 0);
        }
    }
}
