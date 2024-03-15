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
    auto path_list = get_path_list_from_string(path);
    if (path_list.front() == "m") {
        path_list.pop_front();
    } else {
        throw std::runtime_error("Invalid path");
    }
    return internal_search_helper(path_list, root.get(), 0);
}

key_pair hd_trie::internal_search_helper(std::list<std::string> &path_list, hd_node *curr_node, size_t depth) {
    if (path_list.empty()) {
        return curr_node->k_pair;
    }

    hd_node *currnode = curr_node;
    for (const auto &local_path: path_list) {
        size_t index;
        try {
            if (const auto i = local_path.find('\''); i == -1) {//not hardered
                index = std::stoull(local_path);
            } else {
                index = std::stoull(local_path.substr(0, local_path.length() - 1));
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
std::list<std::string> hd_trie::get_path_list_from_string(std::string_view path) {
    const std::string delim = "/";
    std::list<std::string> path_list;
    for (auto &&word_range: std::views::split(path, delim)) {
        std::string word{word_range.begin(), static_cast<std::size_t>(std::ranges::distance(word_range))};
        path_list.emplace_back(word);
    }
    return path_list;
}
