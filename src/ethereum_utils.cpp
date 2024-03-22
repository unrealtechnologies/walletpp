//
// Created by Ariel Saldana on 3/16/24.
//

#include "ethereum_utils.h"
#include "constants.h"

namespace walletpp {
    std::string ethereum_utils::generate_ethereum_address(const Botan::secure_vector<uint8_t> &key) {
        auto pub_key = crypto_algorithms::generate_public_key(key, false);
        if (pub_key.size() == walletpp::public_key_uncompressed_bytes_length && pub_key[0] == walletpp::public_key_uncompressed_form_id) {
            pub_key.erase(pub_key.begin());
        }
        auto hashed_public_key = crypto_algorithms::keccak256(pub_key);
        Botan::secure_vector<uint8_t> last_20_bytes = {hashed_public_key.begin() + 12, hashed_public_key.end()};

        std::string address = Botan::hex_encode(last_20_bytes);

        // transform the characters in the address to lowercase
        for (int i = 0; i < address.length(); i++) {
            address[i] = static_cast<char>(tolower(address[i]));
        }

        return to_checksum_address(address);
    }

    std::string ethereum_utils::to_checksum_address(const std::string &address) {
        std::string checksum_address;
        std::string hash = Botan::hex_encode(crypto_algorithms::keccak256(address));

        for (size_t i = 0; i < address.size(); ++i) {
            if (hash[i] >= '8') {
                checksum_address += static_cast<char>(toupper(address[i]));
            } else {
                checksum_address += address[i];
            }
        }

        crypto_algorithms::secure_erase_string(hash);

        return "0x" + checksum_address;
    }
}// namespace walletpp
