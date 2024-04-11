//
// Created by Ariel Saldana on 3/12/24.
//

#ifndef BIP32_H
#define BIP32_H

#include "bip39.h"
#include "hd_trie.h"
#include "secure_vector.h"


namespace walletpp {
    class bip32 {
        std::pair<std::unique_ptr<extended_key>, std::unique_ptr<extended_key>> master_extended_key_pair_;
        const std::unique_ptr<hd_trie> tree;

    public:
        explicit bip32(std::pair<std::unique_ptr<extended_key>, std::unique_ptr<extended_key>> &&master_extended_key_pair)
            : master_extended_key_pair_(std::move(master_extended_key_pair)), tree(std::make_unique<hd_trie>(std::move(master_extended_key_pair_))) {}


        static bip32 from_seed(const secure_vector<uint8_t> &entropy) {
            auto master_key_pair = master_key_generator::generate_master_key_pair(entropy);
            return bip32(std::move(master_key_pair));
        }

        static bip32 from_seed(const std::array<uint8_t, pbkdf2_sha512_output_byte_size> &entropy_arr) {
            auto master_key_pair = master_key_generator::generate_master_key_pair(secure_vector<uint8_t>{entropy_arr.begin(), entropy_arr.end()});
            return bip32(std::move(master_key_pair));
        }

        [[nodiscard]] auto derive_keypair_with_path(const std::string_view path) const -> std::shared_ptr<hd_node> { return tree->search(path); }
    };
}// namespace walletpp


#endif//BIP32_H
