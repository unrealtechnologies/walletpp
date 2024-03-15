//
// Created by ariel on 3/14/24.
//

#include "hd_trie.h"

#include <iostream>

void hd_trie::initialize_with_seed(const Botan::secure_vector<uint8_t> &seed) {
    const auto kp = walletpp::master_key_generator::generate_master_key_pair(seed);
    root = std::make_unique<hd_node>(*kp);
}

hd_trie::hd_trie(const Botan::secure_vector<uint8_t> &seed) {
    initialize_with_seed(seed);
}

key_pair hd_trie::search(std::string_view path) {
    if (path.empty()) {
        throw std::invalid_argument("Path cannot be empty");
    }

    auto path_list = get_path_list_from_string(path);
    if (path_list.front() == root_identifier) {
        path_list.pop_front();
    } else {
        throw std::runtime_error("Invalid path");
    }

    return internal_search_helper(path_list, root.get(), 0);
}

key_pair hd_trie::internal_search_helper(std::list<std::string_view> &path_list, hd_node *curr_node, size_t depth) {
    if (!curr_node) {
        throw std::invalid_argument("Current node cannot be null");
    }

    if (path_list.empty()) {
        return curr_node->k_pair;
    }

    hd_node *currnode = curr_node;
    for (const auto &local_path: path_list) {
        size_t index;
        try {
            if (const auto i = local_path.find(hardened_key_identifier); i == -1) {//not hardered
                index = std::stoull(std::string(local_path));
            } else {
                index = std::stoull(std::string(local_path.substr(0, local_path.length() - 1)));
            }
        } catch (const std::invalid_argument &ia) {
            std::cerr << "Invalid argument: " << ia.what() << std::endl;
            throw std::runtime_error("Invalid argument error");
        } catch (const std::out_of_range &oor) {
            std::cerr << "Out of range: " << oor.what() << std::endl;
            throw std::runtime_error("Out of range error");
        }

        currnode = currnode->insert_child(index);
    }

    return currnode->k_pair;
}
// Store string views in your list to avoid creating new strings
std::list<std::string_view> hd_trie::get_path_list_from_string(std::string_view path) {
    if (path.find_first_not_of(std::string("0123456789") + std::string(root_identifier) + std::string(path_delimiter) + std::string(hardened_key_identifier)) != std::string_view::npos) {
        throw std::invalid_argument("Path contains invalid characters");
    }
    const std::string_view delim = path_delimiter.data();
    std::list<std::string_view> path_list;
    for (auto &&word_range: std::views::split(path, delim)) {
        path_list.emplace_back(word_range);
    }

    return path_list;
}
