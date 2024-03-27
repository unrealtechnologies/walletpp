//
// Created by ariel on 3/14/24.
//

#ifndef HD_NODE_H
#define HD_NODE_H

#include "key_pair.h"
#include <unordered_map>
#include <utility>

namespace walletpp {
    class hd_node : public std::enable_shared_from_this<hd_node> {
        key_pair k_pair;
        std::unordered_map<size_t, std::shared_ptr<hd_node>> children{};
        std::weak_ptr<hd_node> parent;

    public:
        // this constructor explicitly only allows for moving key_pair
        explicit hd_node(key_pair &&k_pair, std::weak_ptr<hd_node> parent) : k_pair(std::move(k_pair)), parent(std::move(parent)) {}

        hd_node *derive_child(size_t index);
        auto remove_child(size_t index) -> void;
        auto get_key_pair() const -> key_pair;
    };
}// namespace walletpp


#endif//HD_NODE_H
