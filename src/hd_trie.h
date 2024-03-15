//
// Created by ariel on 3/14/24.
//

#ifndef HD_TRIE_H
#define HD_TRIE_H
#include "hd_derivation.h"
#include "hd_node.h"
#include "master_key_generator.h"

#include <algorithm>
#include <botan/allocator.h>
#include <iostream>
#include <list>
#include <ranges>
#include <string>
#include <string_view>


class hd_trie {
    std::unique_ptr<hd_node> root = nullptr;

    void initialize_with_seed(const Botan::secure_vector<uint8_t>& seed) {
        auto kp = walletpp::master_key_generator::generate_master_key_pair(seed);
        root = std::make_unique<hd_node>(*kp);
    }

public:
    explicit hd_trie(const key_pair& k_pair) : root(std::make_unique<hd_node>(k_pair)) {}

    explicit hd_trie(const Botan::secure_vector<uint8_t> &seed) {
        initialize_with_seed(seed);
    }

    key_pair search(std::string_view path) {
        auto path_list = get_path_list_from_string(path);
        if (path_list.front() == "m") {
            path_list.pop_front();
        } else {
            throw std::runtime_error("Invalid path");
        }
        return internal_search_helper(path_list, root.get(), 0);
    }

    key_pair internal_search_helper(std::list<std::string> &path_list, hd_node *curr_node, size_t depth) {
        if (path_list.empty()) {
            return curr_node->k_pair;
        }

        const auto curr_path = path_list.front();
        path_list.pop_front();

        size_t index;
        try {
            if (const auto i = curr_path.find('\''); i == -1) {//not hardered
                index = std::stoull(curr_path);
            } else {
                index = std::stoull(curr_path.substr(0, curr_path.length() - 1));
            }
        } catch (const std::invalid_argument &ia) {
            std::cerr << "Invalid argument: " << ia.what() << std::endl;
            throw std::runtime_error("Invalid argument error");
        } catch (const std::out_of_range &oor) {
            std::cerr << "Out of range: " << oor.what() << std::endl;
            throw std::runtime_error("Out of range error");
        }

        auto next_node = curr_node->insert_child(index);
        return internal_search_helper(path_list, next_node, 0);
    }

    static std::list<std::string> get_path_list_from_string(std::string_view path) {
        const std::string delim = "/";
        std::list<std::string> path_list;
        for (auto &&word_range: std::views::split(path, delim)) {
            std::string word{word_range.begin(), static_cast<std::size_t>(std::ranges::distance(word_range))};
            path_list.emplace_back(word);
        }
        return path_list;
    }
};


#endif//HD_TRIE_H
