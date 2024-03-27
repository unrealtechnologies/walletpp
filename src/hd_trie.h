//
// Created by ariel on 3/14/24.
//

#ifndef HD_TRIE_H
#define HD_TRIE_H

#include "hd_node.h"
#include "master_key_generator.h"
#include <ranges>
#include <vector>

namespace walletpp {
    class hd_trie {
        const std::string_view root_identifier = "m";
        const std::string_view path_delimiter = "/";
        const std::string_view hardened_key_identifier = "'";

        std::shared_ptr<hd_node> root = nullptr;

        void initialize_with_seed(const walletpp::secure_vector<uint8_t> &seed);

        [[nodiscard]] auto internal_search_helper(const std::vector<std::string_view> &path_vector, hd_node *curr_node, size_t depth) const -> hd_node *;
        [[nodiscard]] auto get_path_vector_from_string(std::string_view path) const -> std::vector<std::string_view>;

    public:
        explicit hd_trie(key_pair &&k_pair)
            : root([&]() -> std::shared_ptr<hd_node> {
                  std::weak_ptr<hd_node> null_parent;
                  return std::make_unique<hd_node>(std::move(k_pair), null_parent);
              }()) {}
        explicit hd_trie(const walletpp::secure_vector<uint8_t> &seed);

        [[nodiscard]] auto search(std::string_view path) const -> hd_node *;
    };
}// namespace walletpp


#endif//HD_TRIE_H
