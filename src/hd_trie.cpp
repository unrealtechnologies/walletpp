//
// Created by ariel on 3/14/24.
//

#include "hd_trie.h"

#include <charconv>

namespace walletpp {
    void hd_trie::initialize_with_seed(const secure_vector<uint8_t> &seed) {
        auto [private_ext_key, public_ext_key] = master_key_generator::generate_master_key_pair(seed);
        root = std::make_shared<hd_node>(std::move(private_ext_key), std::move(public_ext_key), std::shared_ptr<hd_node>{});
    }

    hd_trie::hd_trie(const secure_vector<uint8_t> &seed) { initialize_with_seed(seed); }

    std::shared_ptr<hd_node> hd_trie::search(const std::string_view &path) const {
        if (path.empty()) { throw std::invalid_argument("Path cannot be empty"); }

        const auto path_list = get_path_vector_from_string(path);
        if (path_list.front() == root_identifier) {
            if (path_list.size() == 1) { return root; }
        } else {
            throw std::runtime_error("Invalid path");
        }

        return internal_search_helper(path_list, root);
    }

    std::shared_ptr<hd_node> hd_trie::internal_search_helper(const std::vector<std::string_view> &path_vector, std::shared_ptr<hd_node> curr_node) const {
        if (!curr_node) { throw std::invalid_argument("Current node cannot be null"); }

        if (path_vector.empty()) { return curr_node; }

        auto currnode = curr_node;
        for (size_t i = 1; i < path_vector.size(); i++) {
            std::string_view local_path = path_vector.at(i);

            size_t index;
            if (local_path.find(hardened_key_identifier) == std::string_view::npos) {//not hardened
                std::from_chars(local_path.data(), local_path.data() + local_path.size(), index);
            } else {
                std::from_chars(local_path.data(), local_path.data() + local_path.size() - 1, index);
                index += hardened_key_start_index;
            }

            currnode = currnode->derive_child(index);
        }

        return currnode;
    }

    std::vector<std::string_view> hd_trie::get_path_vector_from_string(std::string_view path) const {
        if (path.find_first_not_of(std::string("0123456789") + std::string(root_identifier) + std::string(path_delimiter) + std::string(hardened_key_identifier)) !=
            std::string_view::npos) {
            throw std::invalid_argument("Path contains invalid characters");
        }
        const std::string_view delim = path_delimiter.data();
        std::vector<std::string_view> path_vector;
        for (auto &&word_range: std::views::split(path, delim)) { path_vector.emplace_back(word_range); }

        return path_vector;
    }
}// namespace walletpp
