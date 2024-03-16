//
// Created by ariel on 3/14/24.
//

#ifndef HD_TRIE_H
#define HD_TRIE_H

#include "hd_node.h"
#include "master_key_generator.h"
#include <list>
#include <ranges>

class hd_trie {
    const std::string_view root_identifier = "m";
    const std::string_view path_delimiter = "/";
    const std::string_view hardened_key_identifier = "'";

    std::unique_ptr<hd_node> root = nullptr;

    void initialize_with_seed(const Botan::secure_vector<uint8_t> &seed);
    [[nodiscard]] key_pair internal_search_helper(std::list<std::string_view> &path_list, hd_node *curr_node, size_t depth);
    [[nodiscard]] std::list<std::string_view> get_path_list_from_string(std::string_view path);

public:
    explicit hd_trie(const key_pair &k_pair) : root(std::make_unique<hd_node>(k_pair)) {}
    explicit hd_trie(const Botan::secure_vector<uint8_t> &seed);
    [[nodiscard]] key_pair search(std::string_view path);
};


#endif//HD_TRIE_H
