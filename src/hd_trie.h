//
// Created by ariel on 3/14/24.
//

#ifndef HD_TRIE_H
#define HD_TRIE_H
#include "hd_node.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

#include <iomanip>
#include <iostream>
#include <list>
#include <ranges>
#include <string>
#include <string_view>


class hd_trie {
    std::unique_ptr<hd_node> root = nullptr;

public:
    explicit hd_trie(const key_pair &k_pair) {
        if (root == nullptr) {
            root = std::make_unique<hd_node>(
                    "m",
                    k_pair,
                    nullptr);
        }
    };

    void insert(std::string_view path) {

    }

    void search(std::string_view path) {
        const auto path_vec = get_path_vector_from_string(path);
    }

    hd_node * internal_search_helper(std::pmr::list<std::string>& path_list, hd_node *curr_node, size_t depth) {
        if (path_list.empty()) {
            throw std::runtime_error("Incorrect path");
        }

        std::string curr_path = path_list.front()->str;
        path_list.pop_front();

        if (curr_path == "m" && path_list.size() == 0 && depth == 0) {
            return curr_node;
        }


        size_t index;

        try {
            if (const auto i = curr_path.find('\''); i == -1) { //not hardered
                index = std::stoull(curr_path);
            } else {
                index = std::stoull(curr_path.substr(0, curr_path.length() -1));
            }
        } catch(const std::invalid_argument &ia) {
            std::cerr << "Invalid argument: " << ia.what() << std::endl;
        } catch (const std::out_of_range& oor) {
            std::cerr << "Out of range: " << oor.what() << std::endl;
        }

        auto children = curr_node->children;

    }

    static std::vector<std::string_view> get_path_vector_from_string(std::string_view path) {
        const std::string delim = "/";
        std::vector<std::string_view> path_vector;
        for (auto&& word_range : std::views::split(path, delim)) {
            std::string_view word{word_range.begin(), static_cast<std::size_t>(std::ranges::distance(word_range))};
            path_vector.emplace_back(word);
        }
        return path_vector;
    }
};


#endif//HD_TRIE_H
