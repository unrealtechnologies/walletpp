//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef BIP32_H
#define BIP32_H

#include "bip39.h"
#include "hd_trie.h"
#include "key_pair.h"

#include <utility>

class bip32 {
    const key_pair master_extended_key_pair_;
    const std::unique_ptr<hd_trie> tree;


    key_pair from_base_58(const std::string_view &key);
    key_pair derive_path(const std::string_view &path);

public:
    explicit bip32(key_pair master_extended_key_pair) : master_extended_key_pair_(std::move(master_extended_key_pair)), tree([&]() -> std::unique_ptr<hd_trie> {
                                                            return std::make_unique<hd_trie>(master_extended_key_pair_);
                                                        }()) {}

    static bip32 from_entropy(const Botan::secure_vector<uint8_t> &entropy) {
        const auto master_key_pair = walletpp::master_key_generator::generate_master_key_pair(entropy);
        return bip32(*master_key_pair);// Directly return a bip32 object
    }

    static bip32 from_entropy(const std::array<uint8_t, crypto_algorithms::pbkdf2_sha512_output_byte_size> &entropy_arr) {
        const Botan::secure_vector<uint8_t>entropy{entropy_arr.begin(), entropy_arr.end()};
        const auto master_key_pair = walletpp::master_key_generator::generate_master_key_pair(entropy);
        return bip32(*master_key_pair);// Directly return a bip32 object
    }

    [[nodiscard]] auto derive_keypair_with_path(const std::string_view path) const -> hd_node* {
        return tree->search(path);
    }
};


#endif//BIP32_H
