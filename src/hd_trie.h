//
// Created by ariel on 3/14/24.
//

#ifndef HD_TRIE_H
#define HD_TRIE_H

#include "hd_node.h"
#include "master_key_generator.h"
#include <list>
#include <ranges>
#include <string>

class hd_trie {
    std::unique_ptr<hd_node> root = nullptr;
    void initialize_with_seed(const Botan::secure_vector<uint8_t> &seed);
    static key_pair internal_search_helper(std::list<std::string> &path_list, hd_node *curr_node, size_t depth);
    static std::list<std::string> get_path_list_from_string(std::string_view path);

public:
    explicit hd_trie(const key_pair &k_pair) : root(std::make_unique<hd_node>(k_pair)) {}
    explicit hd_trie(const Botan::secure_vector<uint8_t> &seed);
    key_pair search(std::string_view path);
};


#endif//HD_TRIE_H
