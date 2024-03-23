//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef BIP32_H
#define BIP32_H

#include "bip39.h"
#include "hd_trie.h"
#include "key_pair.h"

namespace walletpp {
    class bip32 {
        const key_pair master_extended_key_pair_;
        const std::unique_ptr<hd_trie> tree;

        key_pair from_base_58(const std::string_view &key);
        key_pair derive_path(const std::string_view &path);

    public:
        explicit bip32(key_pair master_extended_key_pair)
            : master_extended_key_pair_(std::move(master_extended_key_pair)),
              tree([&]() -> std::unique_ptr<hd_trie> { return std::make_unique<hd_trie>(master_extended_key_pair_); }()) {}

        static bip32 from_seed(const Botan::secure_vector<uint8_t> &entropy) {
            const auto master_key_pair = walletpp::master_key_generator::generate_master_key_pair(entropy);
            return bip32(*master_key_pair);
        }

        static bip32 from_seed(const std::array<uint8_t, pbkdf2_sha512_output_byte_size> &entropy_arr) {
            const auto master_key_pair = walletpp::master_key_generator::generate_master_key_pair(Botan::secure_vector<uint8_t>{entropy_arr.begin(), entropy_arr.end()});
            return bip32(*master_key_pair);
        }

        [[nodiscard]] auto derive_keypair_with_path(const std::string_view path) const -> hd_node * { return tree->search(path); }
    };
}// namespace walletpp


#endif//BIP32_H
